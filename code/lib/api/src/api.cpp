#include "api.h"
#include "http_async_api.h"
//#define VIR_POSITION

namespace machyAPI
{
    namespace machysockets_aSync
    {
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

        void service::StartHandling()
        {
            std::cout<<"start service\n";
            asio::async_read_until(*m_sock.get(), m_request, "\n\n", 
                [this]( const boost::system::error_code& ec, std::size_t bytes_transferred)
                {onRequestRecieved(ec, bytes_transferred);});
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
            //machycore::print_vpos_data();
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
            std::istream is(&request);
            std::string line;
            while (is)
            {
                while(std::getline(is, line, '\n'))
                {
                    std::stringstream ss(line);
                    float value[2];
                    for (int i=0; i<2; i++)
                    {
                        ss >> value[i];
                        ss.ignore();
                    }
                    machycore::trajectory->push_back( value );
#ifdef VIR_POSITION
                    float value_vir[3];
                    for (int i=0; i<3; i++)
                    {
                        ss >> value_vir[i];
                        ss.ignore();
                    }
                    machycore::virposition->push_back( value_vir );
#endif
                }
            }
            std::string response("OK\n");
            return response;
        }

/* 
        std::string service::trajectory_ProcessRequest(asio::streambuf& request)
        {
            // clear vector
            machycore::virposition->clear();
            std::istream is(&request);
            std::string line;
            while (is)
            {
                while(std::getline(is, line, '\n'))
                {
                    std::stringstream ss(line);
                    float value[5];
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
        } */
        
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