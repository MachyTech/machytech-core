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
#ifdef BOOST_ENHANCED
                writeToSocketEnhanced(sock);
#else
                writeToSocket(sock);
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
            const unsigned char MESSAGE_SIZE = 5;
            char buf[MESSAGE_SIZE];
            std::size_t total_bytes_read = 0;
            while (total_bytes_read != MESSAGE_SIZE) {
                total_bytes_read += sock.read_some(asio::buffer(buf + total_bytes_read,MESSAGE_SIZE - total_bytes_read));
            }
            return std::string(buf, total_bytes_read);
        }

        std::string readFromSocketEnhanced(asio::ip::tcp::socket& sock)
        {
            const unsigned char MESSAGE_SIZE = 5;
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
    }
    namespace machysockets_aSync
    {
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
        }
    }
}