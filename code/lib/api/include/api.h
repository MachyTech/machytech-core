#ifndef API_H
#define API_H

#include <boost/asio.hpp>
//#include <boost/filesystem.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

#include <fstream>

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
        typedef void(*Callback) (unsigned int request_id, const std::string& response, const system::error_code& ec);
        /*
         * data type representing a pointer to a callback function.
         * used by the asynchronous client application
         */
        struct aSync_Session {
            aSync_Session(asio::io_service& ios,
            const std::string& raw_ip_address,
            unsigned short port_num,
            const std::string& request,
            unsigned int id,
            Callback callback) :
            m_sock(ios),
            m_ep(asio::ip::address::from_string(raw_ip_address),port_num),
            m_request(request),
            m_id(id),
            m_callback(callback),
            m_was_cancelled(false) {}

            asio::ip::tcp::socket m_sock;
            asio::ip::tcp::endpoint m_ep;
            std::string m_request;

            asio::streambuf m_response_buf;
            std::string m_response;

            system::error_code m_ec;

            unsigned int m_id;

            Callback m_callback;

            bool m_was_cancelled;
            std::mutex m_cancel_guard;
        };
        /*
         * keep data related to a particular request
         * while it is being executed
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
        void handler(unsigned int request_id, const std::string& response, const system::error_code& ec);
        /*
         * callback function to output results of the request execution
         */
        int asynchronous_client();
        /*
         * connect to a server in asynchronous manner
         */
        int asynchronous_httpserver();
        /*
         * http server
         */
        class aSyncTCPClient : public boost::noncopyable
        {
            /*
             * all the functionality related to communication
             * with the server application. It spawns a thread
             * that plays the role of I/O thread in the application.
             * In the context of this thread, the callbacks assigned
             * asynchronous operations will be invoked.
             */
            public:
                aSyncTCPClient() {
                    m_work.reset(new asio::io_service::work(m_ios));
                    m_thread.reset(new std::thread([this] (){
                        m_ios.run();
                    }));
                }
                void emulateLongComputationOp(unsigned int duration_sec, 
                    const std::string& raw_ip_address,
                    unsigned short port_num, Callback callback, unsigned int request_id);

                void cancelRequest(unsigned int request_id);
                void close(){
                    m_work.reset(NULL);
                    m_thread->join();
                }
            private:
                void onRequestComplete(std::shared_ptr<aSync_Session> session);
                asio::io_service m_ios;
                std::map<int, std::shared_ptr<aSync_Session>> m_active_sessions;
                std::mutex m_active_sessions_guard;
                std::unique_ptr<boost::asio::io_service::work> m_work;
                std::unique_ptr<std::thread> m_thread;
        };

        class http_service
        {
            /*
             * service class that provides the implementation of the 
             * http protocol.
             */
            static const std::map<unsigned int, std::string>http_status_stable;
            public:
                http_service(std::shared_ptr<boost::asio::ip::tcp::socket> sock) :
                    m_sock(sock), m_request(4096), m_response_status_code(200), m_resource_size_bytes(0)
                {};
                void StartHandling();
                /*
                 * initiate asynchronous communication session with the
                 * client connected to the socket
                 */
            private:
                void on_request_line_recieved(const boost::system::error_code& ec,
                    std::size_t bytes_transferred);
                /*
                 * process the http request line 
                 * supported: GET
                 */
                void on_headers_received(const boost::system::error_code& ec,
                    std::size_t bytes_transferred);
                /*
                 * process and store the request header block
                 */
                void process_request();
                /*
                 * read the contents of the requested resource from
                 * the file system and store it in a buffer
                 */
                void send_response();
                /*
                 * response message and send it to the client
                 */
                void on_response_sent(const boost::system::error_code& ec,std::size_t bytes_transferred);
                /*
                 * after response sent kill the socket and cleanup
                 */
                void on_finish(){ delete this; }
                std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
                boost::asio::streambuf m_request;
                std::map<std::string, std::string> m_request_headers;
                std::string m_requested_resource;

                std::unique_ptr<char[]> m_resource_buffer;
                unsigned int m_response_status_code;
                std::size_t m_resource_size_bytes;
                std::string m_response_headers;
                std::string m_response_status_line;
        };

        const std::map<unsigned int, std::string>
        http_status_table = 
        {
            { 200, "200 OK" },
            { 404, "404 Not Found" },
            { 413, "413 Request Entity Too Large" },
            { 500, "500 Server Error" },
            { 501, "501 Not Implemented" },
            { 505, "505 HTTP Version Not Supported" }
        };

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
                void start_http(){
                    std::cout<<"start http acceptor\n";
                    m_acceptor.listen();
                    InitAccept_http();
                }
                /*
                 * start accepting incoming http connection
                 * requests
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
                void InitAccept_http();
                /*
                 * constructs an active socket object and 
                 * initiates the asynchronous
                 */
                void onAccept(const boost::system::error_code& ec,
                    std::shared_ptr<asio::ip::tcp::socket> sock);
                /*
                * if asynchronous operation was succesfull an instance
                * of the service class is created.
                */
                void onAccept_http(const boost::system::error_code& ec,
                    std::shared_ptr<asio::ip::tcp::socket> sock);
                /*
                 * if asynchronous operation was succesfull an instance
                 * of the http_service class is created
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
                void start_http(unsigned short port_num, unsigned int thread_pool_size);
                /*
                 * starts asynchronous http server on specified port
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