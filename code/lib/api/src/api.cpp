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
                std::this_thread::sleep_for(std::chrono::seconds(60));
                srv.stop();
            }
            catch (system::system_error&e) {
                std::cout << "Error occured! Error code = "<<e.code()
                    <<". Message: "<<e.what();
            }
            return 0;
        }

        void service::StartHandling()
        {
            std::cout<<"start service\n";
            asio::async_read_until(*m_sock.get(), m_request, '\n', 
                [this]( const boost::system::error_code& ec, std::size_t bytes_transferred)
                {onRequestRecieved(ec, bytes_transferred);});
        }

        void service::onRequestRecieved(const boost::system::error_code& ec, std::size_t bytes_transferred)
        {
            std::cout<<"request recieved!\n";
            if (ec != 0) {
                std::cout<< "Error occured! Error code = "<<ec.value()<<". Message: "<<ec.message();
                onFinish();
                return;
            }
            m_response = ProcessRequest(m_request);
            std::cout<<"meanwhile sending response\n";
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

        std::string service::ProcessRequest(asio::streambuf& request)
        {
            int i=0;
            std::cout<<"starting cpu process emulation\n";
            while (i != 10000000)
                i++;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout<<"finished...\n";
            std::string response = "Response\n";
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