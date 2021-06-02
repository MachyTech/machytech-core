#ifndef API_H
#define API_H

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>

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
         * asynchronous writing operation using async_write_some()
         */
        //void readFromSocket_async();
     }
}
#endif