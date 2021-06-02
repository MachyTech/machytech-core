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
     namespace machysockets
     {
        /*
         * machy sockets
         */
        struct Session {
            std::shared_ptr<asio::ip::tcp::socket> sock;
            std::unique_ptr<char[]> buf;
            std::size_t total_bytes_read;
            unsigned int buf_size;
        };
        /*
         * store objects for callback function
         * and identify al data has been read
         */
        void callback(const boost::system::error_code& ec,
            std::size_t bytes_transferred, std::shared_ptr<Session> s);
        /*
         * callback function for asynchronous reading
         */
        //void readFromSocket_async();
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
         * client function to write to socket
         */
        void readFromSocket(asio::ip::tcp::socket& sock);
     }
}
#endif