#include "api.h"

namespace machyAPI
{
    namespace machysockets_Sync
    {
        int create_passive_synchronous_socket()
        {
            // size pending connection request
            const int BACKLOG_SIZE = 30;
            // define a port
            unsigned short port_num = 3333;
            // creating a server endpoint
            asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
            // instance of io service class
            asio::io_service ios;
            try{
                // acceptor socket object
                asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
                // binding socket to endpoint
                acceptor.bind(ep);
                // listen for incoming connection request
                acceptor.listen(BACKLOG_SIZE);
                // create active socket
                asio::ip::tcp::socket sock(ios);
                // connect to client
                acceptor.accept(sock);
                // read from this socket
#ifdef BOOST_ENHANCED
                std::string socketData = readFromSocketEnhanced(sock);
#else
                std::string socketData = readFromSocket(sock);
#endif
                std::cout<<socketData<<std::endl;
            }
            catch(system::system_error &e){
                std::cout<< "Error occured! Error code = " << e.code()
                << ". Message: " << e.what()<<std::endl;
                return e.code().value();
            }
            return 0;
        }

        int connect_synchronous_socket()
        {
            std::string raw_ip_address = "127.0.0.1";
            unsigned short port_num = 3333;

            try {
                // create endpoint to target server
                asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
                asio::io_service ios;
                // creating and opening socket
                asio::ip::tcp::socket sock(ios, ep.protocol());
                // connecting a socket
                sock.connect(ep);
                // writing to the socket
#ifdef BOOST_ENHANCED
                readFromSocketEnhanced(sock);
#else
                readFromSocket(sock);
#endif
            } 
            catch(system::system_error &e){
                std::cout << "Error occured! Error code = " << e.code()
                << ". Message: "<< e.what();
                return e.code().value(); 
            }
            return 0;
        }

        void writeToSocket(asio::ip::tcp::socket& sock)
        {
            // allocating and filling buffer
            std::string buf = "Hello";

            std::size_t total_bytes_written = 0;
            // write all data
            while (total_bytes_written != buf.length()) {
                total_bytes_written += sock.write_some(
                    asio::buffer(buf.c_str() + total_bytes_written, 
                    buf.length() - total_bytes_written));
            }
        }

        void writeToSocketEnhanced(asio::ip::tcp::socket& sock)
        {
            std::string buf = "Hello";
            asio::write(sock, asio::buffer(buf));
        }

        std::string readFromSocket(asio::ip::tcp::socket& sock)
        {
            const unsigned char MESSAGE_SIZE = 9;
            char buf[MESSAGE_SIZE];
            std::size_t total_bytes_read = 0;
            while (total_bytes_read != MESSAGE_SIZE) {
                total_bytes_read += sock.read_some(asio::buffer(buf + total_bytes_read,MESSAGE_SIZE - total_bytes_read));
            }
            return std::string(buf, total_bytes_read);
        }

        std::string readFromSocketEnhanced(asio::ip::tcp::socket& sock)
        {
            const unsigned char MESSAGE_SIZE = 9;
            char buf[MESSAGE_SIZE];
            asio::read(sock, asio::buffer(buf, MESSAGE_SIZE));

            return std::string(buf, MESSAGE_SIZE);
        }

        std::string readFromSocketDelim(asio::ip::tcp::socket& sock)
        {
            asio::streambuf buf;
            // read data from the socket until \n is encounterd
            asio::read_until(sock, buf, '\n');
            std::string message;
            // buffer may contain rubbish after \n
            std::istream input_stream(&buf);
            std::getline(input_stream, message);
            return message;
        }

        int synchronous_client()
        {
            const std::string raw_ip_address = "127.0.0.1";
            const unsigned short port_num = 3333;

            try {
                SyncTCPClient client(raw_ip_address, port_num);
                
                client.connect();
                std::cout<< "Sending request to the server... "<<std::endl;
                
                std::string response = client.emulateLongComputationOp(10);
                std::cout << "Response received: "<<response<<std::endl;
                
                client.close();
            }
            catch (system::system_error &e) {
                std::cout<<"Error occured! Error code = "<<e.code()<<". Message: "<<e.what();
                return e.code().value();
            }

            return 0;
        }
        void SyncTCPClient::close()
        {
            m_sock.shutdown(asio::ip::tcp::socket::shutdown_both);
            m_sock.close();
        }

        std::string SyncTCPClient::emulateLongComputationOp (unsigned int duration_sec)
        {
            std::string request = " EMULATE_LONG_COMP_OP " + std::to_string(duration_sec) + "\n";
            sendRequest(request);
            return receiveResponse();
        }

        std::string SyncTCPClient::receiveResponse()
        {
            asio::streambuf buf;
            asio::read_until(m_sock, buf, '\n');
            std::istream input(&buf);
            std::string response;
            std::getline(input, response);
            
            return response;
        }
    }

    namespace machysockets_aSync
    {
        void read_callback(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<read_Session> s)
        {
            if (ec != 0) {
                std::cout << "Error occured! Error code = "<< ec.value() << ". Message: "<< ec.message();
                return;
            }

            s->total_bytes_read += bytes_transferred;

            if(s->total_bytes_read == s->buf_size)
            {
                return;
            }

            s->sock->async_read_some(
                asio::buffer(
                s->buf.get() +
                s->total_bytes_read,
                s->buf_size -
                s->total_bytes_read),
                std::bind(read_callback, std::placeholders::_1,
                std::placeholders::_2, s));
        }

        void write_callback(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<write_Session> s)
        {
            if (ec != 0) {
                std::cout << "Error occured! Error code = "<< ec.value() << ". Message: "<< ec.message();
                return;
            }

            s->total_bytes_written += bytes_transferred;
            if (s->total_bytes_written == s->buf.length()) {
                return;
            }
            s->sock->async_write_some(
                asio::buffer(s->buf.c_str() + s->total_bytes_written,
                                s->buf.length() - s->total_bytes_written),
                std::bind(write_callback, std::placeholders::_1,
                            std::placeholders::_2, s)
            );
        }

        void readFromSocket(std::shared_ptr<asio::ip::tcp::socket> sock)
        {
            std::shared_ptr<read_Session> s(new read_Session);

            const unsigned int MESSAGE_SIZE = 9;

            s->buf.reset(new char[MESSAGE_SIZE]);
            s->total_bytes_read = 0;
            s->sock = sock;
            s->buf_size = MESSAGE_SIZE;
            
            s->sock->async_read_some(
                asio::buffer(s->buf.get(), s->buf_size),
                std::bind(read_callback,
                        std::placeholders::_1,
                        std::placeholders::_2,
                        s)
            );
        }

        void writeToSocket(std::shared_ptr<asio::ip::tcp::socket> sock)
        {
            std::shared_ptr<write_Session> s(new write_Session);

            s->buf = std::string("Hello");
            s->total_bytes_written  = 0;
            s->sock = sock;

            s->sock->async_write_some(
                asio::buffer(s->buf),
                std::bind(write_callback, std::placeholders::_1, std::placeholders::_2, s)
            );
            std::cout<<"i am doing something while writing"<<std::endl;
        }

        int connect_asynchronous_socket()
        {
            std::string raw_ip_address = "127.0.0.1";
            unsigned short port_num = 3333;

            try{
                asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
                asio::io_service ios;
                std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));
                new asio::ip::tcp::socket(ios, ep.protocol());

                sock->connect(ep);

                readFromSocket(sock);

                ios.run();
            }
            catch(system::system_error &e) {
                std::cout<< "Error occured! Error code = "<<e.code()
                    << ". Message: "<<e.what();
                return e.code().value(); 
            }
            return 0;
        }

        int create_asynchronous_socket()
        {
            // size pending connection request
            const int BACKLOG_SIZE = 30;
            // define a port
            unsigned short port_num = 3333;
            // creating a server endpoint
            asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
            // instance of io service class
            asio::io_service ios;
            try{
                // acceptor socket object
                asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
                // binding socket to endpoint
                acceptor.bind(ep);
                // listen for incoming connection request
                acceptor.listen(BACKLOG_SIZE);
                // create active socket
                asio::ip::tcp::socket sock(ios);
                // connect to client
                acceptor.accept(sock);
                //acceptor.async_accept(sock, accept_handler);
                //readFromSocket(sock);
                ios.run();
            }
            catch(system::system_error &e) {
                std::cout<< "Error occured! Error code = "<<e.code()
                    << ". Message: "<<e.what();
                return e.code().value();
            }
            return 0;
        }

        int asynchronous_server()
        {
            unsigned short port_num = 3333;

            try {
                server srv;

                unsigned int thread_pool_size = std::thread::hardware_concurrency() * 2;
                if (thread_pool_size == 0)
                    thread_pool_size = DEFAULT_THREAD_POOL_SIZE;
                
                srv.start(port_num, thread_pool_size);
                std::this_thread::sleep_for(std::chrono::seconds(5));
                srv.stop();
            }
            catch (system::system_error&e) {
                std::cout << "Error occured! Error code = "<<e.code()
                    <<". Message: "<<e.what();
            }
            return 0;
        }

        int asynchronous_server(std::string port)
        {
            unsigned short port_num = std::stoi(port);

            try {
                server srv;

                unsigned int thread_pool_size = std::thread::hardware_concurrency() * 2;
                if (thread_pool_size == 0)
                    thread_pool_size = DEFAULT_THREAD_POOL_SIZE;
                
                srv.start(port_num, thread_pool_size);
                while(1){}
            }
            catch (system::system_error&e) {
                std::cout << "Error occured! Error code = "<<e.code()
                    <<". Message: "<<e.what();
            }
            return 0;
        }

        void handler(unsigned int request_id, const std::string& response, const system::error_code& ec)
        {
            if (ec == 0) {
                std::cout << "Request #"<<request_id<<" has completed. Response: "
                    << response << std::endl;
            } else if (ec == asio::error::operation_aborted) {
                std::cout << "Request #" << request_id << " has been cancelled by the user.\n";
            } else {
                std::cout << "Request #"<< request_id << " failed! Error code = " << ec.value()
                <<". Error message = "<<ec.message()<<std::endl;
            }

            return;
        }

        int asynchronous_client()
        {
            try {
                aSyncTCPClient client;

                client.emulateLongComputationOp(10, "127.0.0.1", 3333, handler, 1);
                std::this_thread::sleep_for(std::chrono::seconds(2));
                client.emulateLongComputationOp(11, "127.0.0.1", 3333, handler, 2);
                client.cancelRequest(1);
                std::this_thread::sleep_for(std::chrono::seconds(6));
                client.emulateLongComputationOp(12, "127.0.0.1", 3333, handler, 3);
                std::this_thread::sleep_for(std::chrono::seconds(15));
                client.close();
            }
            catch (system::system_error &e) {
                std::cout << "Error occured! Error code = " << e.code() << ". Message: "<< e.what();
                return e.code().value();
            }
            return 0;
        }

        int asynchronous_httpserver()
        {
            unsigned short port_num = 3333;

            try {
                server srv;

                unsigned int thread_pool_size = std::thread::hardware_concurrency() * 2;
                if (thread_pool_size == 0)
                    thread_pool_size = DEFAULT_THREAD_POOL_SIZE;
                
                srv.start_http(port_num, thread_pool_size);
                std::this_thread::sleep_for(std::chrono::seconds(60));
                srv.stop();
            }
            catch (system::system_error &e) {
                std::cout << "Error occured! Error code = "<<e.code()
                    <<". Message: "<<e.what();
            }
            return 0;
        }

        void aSyncTCPClient::emulateLongComputationOp(unsigned int duration_sec, 
            const std::string& raw_ip_address,
            unsigned short port_num,
            Callback callback,
            unsigned int request_id)
        {
            std::string request = "EMULATE_LONG_CALC_OP " + std::to_string(duration_sec) + "\n";
            std::shared_ptr<aSync_Session> session =
            std::shared_ptr<aSync_Session>(new aSync_Session(m_ios, 
                raw_ip_address, port_num, request, request_id, callback));
            session->m_sock.open(session->m_ep.protocol());

            std::unique_lock<std::mutex> lock(m_active_sessions_guard);
            m_active_sessions[request_id] = session;
            lock.unlock();
            session->m_sock.async_connect(session->m_ep,
                [this, session](const system::error_code& ec)
                {
                    if (ec != 0) {
                        session->m_ec = ec;
                        onRequestComplete(session);
                        return;
                    }
                    std::unique_lock<std::mutex>cancel_lock(session->m_cancel_guard);
                    if(session->m_was_cancelled){
                        onRequestComplete(session);
                        return;
                    }
                    asio::async_write(session->m_sock, asio::buffer(session->m_request),
                    [this, session](const boost::system::error_code& ec,
                                        std::size_t bytes_transferred)
                        {
                            if(ec != 0) {
                                session->m_ec = ec;
                                onRequestComplete(session);
                                return;
                            }
                            std::unique_lock<std::mutex>cancel_lock(session->m_cancel_guard);
                            if (session->m_was_cancelled) {
                                onRequestComplete(session);
                                return;
                            }
                            asio::async_read_until(session->m_sock, session->m_response_buf,'\n',
                            [this, session](const boost::system::error_code& ec,
                                std::size_t bytes_transferred)
                                {
                                    if (ec != 0) {
                                        session->m_ec = ec;
                                    } else {
                                        std::istream strm(&session->m_response_buf);
                                        std::getline(strm, session->m_response);
                                    }

                                    onRequestComplete(session);
                                });//async_read_until
                        });// async_write
                });// async_connect
        }

        void aSyncTCPClient::cancelRequest(unsigned int request_id) {
            std::unique_lock<std::mutex> lock(m_active_sessions_guard);
            auto it = m_active_sessions.find(request_id);
            if (it != m_active_sessions.end()) {
                std::unique_lock<std::mutex>cancel_lock(it->second->m_cancel_guard);
                it->second->m_was_cancelled = true;
                it->second->m_sock.cancel();
            }
        }

        void aSyncTCPClient::onRequestComplete(std::shared_ptr<aSync_Session> session)
        {
            boost::system::error_code ignored_ec;
            session->m_sock.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
            std::unique_lock<std::mutex> lock(m_active_sessions_guard);
            auto it = m_active_sessions.find(session->m_id);
            if (it != m_active_sessions.end())
                m_active_sessions.erase(it);
            lock.unlock();
            boost::system::error_code ec;
            if (session->m_ec == 0 && session->m_was_cancelled)
                ec = asio::error::operation_aborted;
            else
                ec = session->m_ec;
            session->m_callback(session->m_id, session->m_response, ec);
        }

        void http_service::StartHandling()
        {
            asio::async_read_until(*m_sock.get(), m_request, "\r\n",
                [this](
                    const boost::system::error_code& ec,
                    std::size_t bytes_transferred)
                {on_request_line_recieved(ec, bytes_transferred);}); //async_read_until
        }

        void http_service::on_request_line_recieved(const boost::system::error_code& ec,
                    std::size_t bytes_transferred)
        {
            if(ec != 0) {
                std::cout<< "Error occured! Error code = "<<ec.value()
                << ". Message: " <<ec.message();

                if(ec == asio::error::not_found){
                    m_response_status_code = 413;
                    send_response();

                    return;
                }
                else {
                    on_finish();
                    return;
                }
            }
            std::string request_line;
            std::istream request_stream(&m_request);
            std::getline(request_stream, request_line, '\r');
            request_stream.get();

            std::string request_method;
            std::istringstream request_line_stream(request_line);
            request_line_stream >> request_method;

            if (request_method.compare("GET") != 0) {
                m_response_status_code = 501;
                send_response();

                return;
            }

            std::cout<<"[GET]\n";
            request_line_stream >> m_requested_resource;

            std::string request_http_version;
            request_line_stream >> request_http_version;

            if (request_http_version.compare("HTTP/1.1") != 0) {
                m_response_status_code = 505;
                send_response();
                
                return;
            }
            asio::async_read_until(*m_sock.get(), m_request, "\r\n\r\n",
            [this]( const boost::system::error_code& ec,
                            std::size_t bytes_transferred)
                {
                    on_headers_received(ec, bytes_transferred);
                });

            return;
        }

        void http_service::on_headers_received(const boost::system::error_code& ec,
            std::size_t bytes_transferred)
        {
            if( ec != 0){
                std::cout<< "Error occured! Error code = "<<ec.value()<<". Message: "<<ec.message();

                if(ec == asio::error::not_found){
                    m_response_status_code = 413;
                    send_response();
                    return;
                }
                else {
                    on_finish();
                    return;
                }
            }

            std::istream request_stream(&m_request);
            std::string header_name, header_value;

            while(!request_stream.eof()) {
                std::getline(request_stream, header_name, ':');
                if (!request_stream.eof()){
                    std::getline(request_stream, header_value, 'r');
                    request_stream.get();
                    m_request_headers[header_name] = header_value;
                }
            }

            process_request();
            send_response();

            return;
        }

        void service::StartHandling()
        {
            std::cout<<"start service\n";
            asio::async_read_until(*m_sock.get(), m_request, "\n\n", 
                [this]( const boost::system::error_code& ec, std::size_t bytes_transferred)
                {onRequestRecieved(ec, bytes_transferred);});
        }

        void http_service::process_request()
        {
            std::string resource_file_path = std::string("/home/timo/" + m_requested_resource);
/* if (!boost::filesystem::exists(resource_file_path)) {
                m_response_status_code = 404;
                return;
            } */
            std::ifstream ifs;
            ifs.open(resource_file_path, std::ifstream::binary);
            if(ifs.fail())
            {
                std::cout<<"file not found\n";
                m_response_status_code = 404;
                return;
            }
            if (!ifs.good())
            {
                std::cout<<"file not opened ifstream\n";
                m_response_status_code = 500;
                return;
            }
            //std::ifstream resource_fstream(resource_file_path, std::ifstream::binary);
            // get length of file
            ifs.seekg(0, std::ifstream::end);
            m_resource_size_bytes = static_cast<std::size_t>(ifs.tellg());
            m_resource_buffer.reset(new char[m_resource_size_bytes]);

            ifs.seekg(std::ifstream::beg);
            ifs.read(m_resource_buffer.get(), m_resource_size_bytes);

            m_response_headers += std::string("content-length") + ": " 
            + std::to_string(m_resource_size_bytes) + "\r\n";
        }

        void http_service::send_response() {
            m_sock->shutdown(asio::ip::tcp::socket::shutdown_receive);
            auto status_line = http_status_table.at(m_response_status_code);
            m_response_status_line = std::string(" HTTP/1.1 ") + status_line + "\r\n";
            m_response_headers += "\r\n";

            std::vector<asio::const_buffer> response_buffers;
            response_buffers.push_back(asio::buffer(m_response_status_line));
            if (m_response_headers.length() > 0) {
                response_buffers.push_back(asio::buffer(m_response_headers));
            }

            if (m_resource_size_bytes > 0) {
                response_buffers.push_back(asio::buffer(m_resource_buffer.get(), m_resource_size_bytes));
            }

            asio::async_write(*m_sock.get(), response_buffers,
                [this](
                    const boost::system::error_code& ec,
                    std::size_t bytes_transferred)
                {
                    on_response_sent(ec, bytes_transferred);
            }); //async_write
        }

        void http_service::on_response_sent(const boost::system::error_code& ec,std::size_t bytes_transferred)
        {
            if (ec != 0) {
                std::cout<<"Error occured! Error code = "<<ec.value()<<". Message: "<<ec.message();
            }
            m_sock->shutdown(asio::ip::tcp::socket::shutdown_both);
            on_finish();
        }

        void service::onRequestRecieved(const boost::system::error_code& ec, std::size_t bytes_transferred )
        {
            std::cout<<"request recieved!\n";
            if (ec != 0) {
                std::cout<< "Error occured! Error code = "<<ec.value()<<". Message: "<<ec.message();
                onFinish();
                return;
            }
            trajectory_ProcessRequest(m_request);
            machycore::print_vpos_data();
            m_response = "OK\n";
            asio::async_write(*m_sock.get(), asio::buffer(m_response),
                    [this]( const boost::system::error_code& ec, std::size_t bytes_transferred)
                        { onResponseSend(ec, bytes_transferred);});
        }

        void service::onResponseSend(const boost::system::error_code& ec, std::size_t bytes_transferred)
        {
            if (ec != 0) {
                std::cout<< "Error occured! Error code = "<<ec.value()<<". Message: "<<ec.message();
            }
            onFinish();
        }

        std::string service::dummy_ProcessRequest(asio::streambuf& request)
        {
            int i=0;
            std::ostringstream ss;
            ss << &request;
            std::string s = ss.str();
            std::cout<<"recieved: "<<s<<std::endl;

            std::cout<<"starting cpu process emulation\n";
            while (i != 10000000)
                i++;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout<<"finished...\n";
            return s;
        }

        std::string service::trajectory_ProcessRequest(asio::streambuf& request)
        {
            // clear vector
            machycore::virposition->clear();
            std::istream is(&request);
            std::string line;
            float value[5];
            while (is)
            {
                while(std::getline(is, line, '\n'))
                {
                    std::stringstream ss(line);
                    for (int i=0; i<5; i++)
                    {
                        ss >> value[i];
                        ss.ignore();
                    }
                    machycore::virposition->push_back( new machycore::Sim( value ));
                }
            }
            std::string response("OK\n");
            return response;
        }

        void acceptor::InitAccept()
        {
            std::shared_ptr<asio::ip::tcp::socket> 
                sock(new asio::ip::tcp::socket(m_ios));
            m_acceptor.async_accept(*sock.get(), 
                [this, sock](const boost::system::error_code& error)
                {onAccept(error, sock);});
        }

        void acceptor::InitAccept_http()
        {
            std::shared_ptr<asio::ip::tcp::socket>
                sock(new asio::ip::tcp::socket(m_ios));
            m_acceptor.async_accept(*sock.get(),
                [this, sock](const boost::system::error_code& error)
                {onAccept_http(error, sock);});
        }

        void acceptor::onAccept(const boost::system::error_code& ec,
            std::shared_ptr<asio::ip::tcp::socket> sock)
        {
            if (ec == 0) {
                (new service(sock))->StartHandling();
            }
            else {
                std::cout<< "Error occured! Error code = "<<ec.value()<< ". Message: "<<ec.message();
            }
            // init next async accept operation
            if (!m_isStopped.load()) {
                InitAccept();
            }
            else {
                // stop accepting incoming connections
                m_acceptor.close();
            }
        }

        void acceptor::onAccept_http(const boost::system::error_code& ec,
            std::shared_ptr<asio::ip::tcp::socket> sock)
        {
            if (ec == 0) {
                (new http_service(sock))->StartHandling();
            }
            else {
                std::cout<< "Error occured! Error code = "<<ec.value()<< ". Message: "<<ec.message();
            }
            // init next async accept operation
            if (!m_isStopped.load()) {
                InitAccept_http();
            }
            else {
                // stop accepting incoming connections
                m_acceptor.close();
            }
        }
        void server::start(unsigned short port_num, unsigned int thread_pool_size)
        {
            std::cout<<"start server\n";
            assert(thread_pool_size > 0);
            // create and start acceptor
            acc.reset(new acceptor(m_ios, port_num));
            acc->start();
            // create specified number of threads and add them to the pool
            for (unsigned int i = 0; i < thread_pool_size; i++)
            {
                std::unique_ptr<std::thread> th(
                    new std::thread([this]()
                    {
                        m_ios.run();
                    }));
                m_thread_pool.push_back(std::move(th));
            }
        }

        void server::start_http(unsigned short port_num, unsigned int thread_pool_size)
        {
            std::cout<<"start server\n";
            assert(thread_pool_size > 0);
            // create and start acceptor
            acc.reset(new acceptor(m_ios, port_num));
            acc->start_http();
            for (unsigned int i = 0; i<thread_pool_size; i++)
            {
                std::unique_ptr<std::thread> th(
                    new std::thread([this]()
                    {
                        m_ios.run();
                    }));
                m_thread_pool.push_back(std::move(th));
            }
        }

        void server::stop()
        {
            acc->stop();
            m_ios.stop();

            for (auto& th : m_thread_pool)
            {
                th->join();
            }
        }
    }
}