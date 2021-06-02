#include "api.h"

namespace machyAPI
{
    namespace machysockets
    {
        int create_passive_synchronous_socket()
        {
            // size pending connection request
            const int BACKLOG_SIZE = 30;
            // define a port
            unsigned short port_num = 3334;
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
                readFromSocket(sock);
            }
            catch(system::system_error &e){
                std::cout<< "Error occured! Error code = " << e.code()
                << ". Message: " << e.what();
                return e.code().value();
            }
            return 0;
        }

        int connect_synchronous_socket()
        {
            std::string raw_ip_address = "127.0.0.1";
            unsigned short port_num = 3334;

            try {
                // create endpoint to target server
                asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
                
                asio::io_service ios;
                // creating and opening socket
                asio::ip::tcp::socket sock(ios, ep.protocol());
                // connecting a socket
                sock.connect(ep);
                // writing to the socket
                writeToSocket(sock);
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
            std::cout<<"writing : "<<buf<<std::endl;
            // write all data
            while (total_bytes_written != buf.length()) {
                total_bytes_written += sock.write_some(
                    asio::buffer(buf.c_str() + total_bytes_written, 
                    buf.length() - total_bytes_written));
            }
            std::cout<<"succesfull"<<std::endl;
        }

        void readFromSocket(asio::ip::tcp::socket& sock)
        {
            const unsigned char MESSAGE_SIZE = 7;
            char buf[MESSAGE_SIZE];
            std::size_t total_bytes_read = 0;

            while (total_bytes_read != MESSAGE_SIZE) {
                total_bytes_read += sock.read_some(
                asio::buffer(buf + total_bytes_read,
                MESSAGE_SIZE - total_bytes_read));
            }
            std::cout<<"recieved something!"<<std::endl;
            //return std::string(buf, total_bytes_read);
        }

        void callback(const boost::system::error_code& ec,
            std::size_t bytes_transferred,
            std::shared_ptr<Session> s)
        {
            if (ec != 0){
                std::cout << "Error occured! Error code = "<<ec.value()
                        << ". Message: "<< ec.message();
                return; 
            }
            s->total_bytes_read += bytes_transferred;

            if (s->total_bytes_read == s->buf_size) {
                return;
            }

            s->sock->async_read_some(
                asio::buffer(s->buf.get() + s->total_bytes_read, 
                    s->buf_size - s->total_bytes_read),
                std::bind(callback, std::placeholders::_1,
                    std::placeholders::_2, s));
        }
    }
}