#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define HTTP_CLIENT 0
#define TCP_CLIENT 1
#define UNDEFINED_CLIENT 2

class chat_message
{
    public:
        enum { header_length = 4 };
        enum { max_body_length = 512 };
        enum { body_length = 5 };

        chat_message() : body_length_(0)
        {}

        const char* data() const
        {
            return data_;
        }

        const int type() const
        {
            return type_;
        }

        int type()
        {
            return type_;
        }

        char* data()
        {
            return data_;
        }

        std::size_t length() const
        {
            return header_length + body_length_;
        }

        const char* body() const
        {
            return data_ + header_length;
        }

        char *body()
        {
            return data_ + header_length;
        }

        bool decode_body()
        {
            char body[body_length] = "";
            std::strncat(body, &data_[header_length], body_length);
            std::cout <<"body : "<< body << std::endl;
            return true;
        }

        bool decode_header()
        {
            char header[header_length + 1] = "";
            std::strncat(header, data_, header_length);
            std::cout<<"header : "<<header<<std::endl;
            if(std::strcmp(header, "GET ")==0){    
                std::cout<<"http mode\n";
                // is this a websocket or normal GET request?
                type_ = HTTP_CLIENT;
            }
            if(std::strcmp(header,"TCP ")==0){
                std::cout<<"tcp mode\n";
                // here we can also make more messages
                type_ = TCP_CLIENT;
            }
            else {type_= UNDEFINED_CLIENT;}
            return true;
        }

        void encode_header()
        {
            char header[header_length + 1] = "";
            std::sprintf(header, "%4d", static_cast<int>(body_length_));
            std::memcpy(data_, header, header_length);
        }

    private:
        char data_[header_length + max_body_length];
        int type_;
        std::size_t body_length_;
};

#endif // CHAT_MESSAGE_HPP