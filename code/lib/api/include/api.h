#ifndef API_H
#define API_H

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <atomic>

const unsigned int DEFAULT_THREAD_POOL_SIZE = 2;

using namespace boost;

namespace machyAPI
{
    /*
     * machytech API
     */
     namespace machysockets_Sync
     {
        /*
         * machy synchronous sockets
         */
        int create_passive_synchronous_socket();
        /*
         * a passive synchronous socket that waits
         * for incoming connections
         */
        int connect_synchronous_socket();
        /*
         * function to connect to a synchronous socket
         */
        void writeToSocket(asio::ip::tcp::socket& sock);
        /*
         * function to write to socket using boost write_some method
         */
        void writeToSocketEnhanced(asio::ip::tcp::socket& sock);
        /*
         * function to write to socket using boost write method
         */
        std::string readFromSocket(asio::ip::tcp::socket& sock);
        /*
         * function to read from socket using boost read_some method
         */
        std::string readFromSocketEnhanced(asio::ip::tcp::socket& sock);
        /*
         * function to read from socket using boost read method
         */
        std::string readFromSocketDelim(asio::ip::tcp::socket& sock);
        /*
         * function to read from socket until character
         */
        int synchronous_client();
        /*
         * a synchronous client function
         */
        class SyncTCPClient {
            /* 
             * a synchronous tcp client
             */
            public:
                SyncTCPClient(const std::string& raw_ip_address, unsigned short port_num) :
                    m_ep(asio::ip::address::from_string(raw_ip_address), port_num), m_sock(m_ios)
                {
                    m_sock.open(m_ep.protocol());
                }
                void connect() {m_sock.connect(m_ep);}
                void close();
                std::string emulateLongComputationOp (unsigned int duration_sec);
            private:
                void sendRequest(const std::string& request)
                 {asio::write(m_sock, asio::buffer(request));}
                std::string receiveResponse();
                asio::io_service m_ios;               
                asio::ip::tcp::endpoint m_ep;
                asio::ip::tcp::socket m_sock;
        };
     }

     namespace machysockets_aSync
     {
        /*
         * machy asynchronous sockets
         */
        struct read_Session {
            std::shared_ptr<asio::ip::tcp::socket> sock;
            std::unique_ptr<char[]> buf;
            std::size_t total_bytes_read;
            unsigned int buf_size;
        };
        /*
         * store objects for callback function
         * and identify al data has been read
         */
        struct write_Session {
            std::shared_ptr<asio::ip::tcp::socket> sock;
            std::string buf;
            std::size_t total_bytes_written;
        };
        /*
         * store objects for callback function
         * and identify al data has been written
         */
        void read_callback(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<read_Session> s);
        /*
         * callback function for asynchronous reading
         */
        void write_callback(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<write_Session> s);
        /*
         * callback function for asynchronous writing
         */
        void writeToSocket(std::shared_ptr<asio::ip::tcp::socket> sock);
        /*
         * asynchronous writing operation using boosts async_write_some
         */
        void readFromSocket(std::shared_ptr<asio::ip::tcp::socket> sock);
        /*
         * asynchronous reading operation using boosts async_read_some
         */
        int connect_asynchronous_socket();
        /*
         * connect to asynchronous socket
         */
        int asynchronous_server();
        /*
         * create an asynchronous server
         */
        class service
        {
            /*
            * the service class implements the function
            * provided by the server to the clients.
            * One instance of this class is intended
            * to handle a single client by reading the
            * request, processing it, and then sending
            * back the response message.
            */
            public:
                service(std::shared_ptr<asio::ip::tcp::socket> sock) : m_sock(sock)
                {}
                void StartHandling();
                /*
                * this method starts handling the client by initiating
                * the asynchronous reading operation to read the request
                * message from the client specifying the onRequestReceived()
                * method as a callback.
                */
            private:
                void onRequestRecieved(const boost::system::error_code& ec, std::size_t bytes_transferred);
                /*
                * checks for succesfull request then processrequest is
                * called to prepare response message and the asynchronous
                * writing operation is initiated
                */
                void onResponseSend(const boost::system::error_code& ec, std::size_t bytes_transferred);
                /*
                * check request operation succeeded then cleanup
                */
                void onFinish(){ delete this; };
                /*
                * cleanup
                */
                std::string ProcessRequest(asio::streambuf& request);
                /*
                * this implements the service. Default is a dummy service
                * that runs a dummy loop performing one million increment
                * operations.
                */
                std::shared_ptr<asio::ip::tcp::socket> m_sock;
                std::string m_response;
                asio::streambuf m_request;
        };
        
        class acceptor
        {
            /*
            * the acceptor class accepts the connection
            * request from clients and instantiating 
            * objects of the service class
            */
            public:
                acceptor(asio::io_service&ios, unsigned short port_num) :
                    m_ios(ios), 
                    m_acceptor(m_ios, asio::ip::tcp::endpoint(asio::ip::address_v4::any(), port_num)), 
                    m_isStopped(false)
                {}
                void start(){
                    std::cout<<"start acceptor\n";
                    m_acceptor.listen();
                    InitAccept();
                }
                /*
                * start accepting incoming connection requests
                */
                void stop(){ m_isStopped.store(true);}
                /*
                * stop accepting incoming connection requests
                */
            private:
                void InitAccept();
                /*
                * constructs an active socket object and initiates
                * the asynchronous accept operation.
                */
                void onAccept(const boost::system::error_code& ec,
                    std::shared_ptr<asio::ip::tcp::socket> sock);
                /*
                * if asynchronous operation was succesfull an instance
                * of the service class is created.
                */
                asio::io_service&m_ios;
                asio::ip::tcp::acceptor m_acceptor;
                std::atomic<bool>m_isStopped;
        };

        class server
        {
            /*
            * the server class represents the server
            * itself, using the acceptor and service
            * classes.
            */
            public:
                server()
                {
                    m_work.reset(new asio::io_service::work(m_ios));
                }
                void start(unsigned short port_num, unsigned int thread_pool_size);
                /*
                * starts a server on specified port and listens for asynchronous
                * operations on defined number of threads.
                */
                void stop();
                /*
                * stops the acceptor instance and stops all trheads
                * defined in start.
                */
            private:
                asio::io_service m_ios;
                std::unique_ptr<asio::io_service::work>m_work;
                std::unique_ptr<acceptor>acc;
                std::vector<std::unique_ptr<std::thread>>m_thread_pool;
        };
    }
}
#endif