#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define GUI_CLIENT 0
#define VISION_CLIENT 1
#define GRAPHIX_CLIENT 2
#define UNDEFINED_CLIENT 3
#define MACHYTECHCORE 4

class chat_message
{
    public:
        enum { header_length = 4 };
        enum { max_body_length = 512 };
        enum { body_length = 5 };
        enum { target_length = 4 };

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

        const int target() const
        {
            return target_;
        }

        int target()
        {
            return target_;
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
            return header_length + target_length + body_length;
        }

        const char* body() const
        {
            return data_ + header_length;
        }

        char *body()
        {
            return data_ + header_length + target_length;
        }

        bool decode_body()
        {
            char body[body_length+1] = "";
            std::strncat(body, &data_[header_length+target_length], body_length);
            std::cout <<"body : "<< body << std::endl;
            return true;
        }

        bool decode_target()
        {
            char target[target_length+1] = "";
            std::strncat(target, &data_[header_length], target_length);
            std::cout<<"target : "<< target <<std::endl;
            if(std::strcmp(target, "MGUI")==0){    
                std::cout<<"message for machygui\n";
                // set the connection type
                target_ = GUI_CLIENT;
            }
            if(std::strcmp(target,"MVIS")==0){
                std::cout<<"message for machyvision\n";
                // set the connection type
                target_ = VISION_CLIENT;
            }
            if(std::strcmp(target, "MGRX")==0){
                std::cout<<"message for machygraphix\n";
                // set the connection type
                target_ = GRAPHIX_CLIENT;
            }
            if(std::strcmp(target, "MCOR")==0){
                std::cout<<"message for machycore\n";
                // set the connection type
                target_ = MACHYTECHCORE;
            }
            else {target_= UNDEFINED_CLIENT;}
            return true;
        }

        bool decode_header()
        {
            char header[header_length + 1] = "";
            char target[target_length+1] = "";
            std::strncat(header, data_, header_length);
            std::strncat(target, &data_[header_length], target_length);
            std::cout<<"header : "<<header<<std::endl;
            std::cout<<"target : "<<target<<std::endl;

            // some ugly if statements
            if(std::strcmp(header, "MGUI")==0){    
                std::cout<<"message from gui\n";
                // set the connection type
                type_ = GUI_CLIENT;
            }
            if(std::strcmp(header,"MVIS")==0){
                std::cout<<"message from vision\n";
                // set the connection type
                type_ = VISION_CLIENT;
            }
            if(std::strcmp(header, "MGRX")==0){
                std::cout<<"message from machygraphix\n";
                // set the connection type
                type_ = GRAPHIX_CLIENT;
            }
            if(std::strcmp(header, "MCOR")==0){
                std::cout<<"message from machycore\n";
                // set the connection type
                type_ = MACHYTECHCORE;
            }

            // some more terrible if statements
            if(std::strcmp(target, "MGUI")==0){    
                std::cout<<"message for machygui\n";
                // set the connection type
                target_ = GUI_CLIENT;
            }
            if(std::strcmp(target,"MVIS")==0){
                std::cout<<"message for machyvision\n";
                // set the connection type
                target_ = VISION_CLIENT;
            }
            if(std::strcmp(target, "MGRX")==0){
                std::cout<<"message for machygraphix\n";
                // set the connection type
                target_ = GRAPHIX_CLIENT;
            }
            if(std::strcmp(target, "MCOR")==0){
                std::cout<<"message for machycore\n";
                // set the connection type
                target_ = MACHYTECHCORE;
            }

            else {
                type_= UNDEFINED_CLIENT;
                target_ = UNDEFINED_CLIENT;}
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
        int target_;
        std::size_t body_length_;
};

#endif // CHAT_MESSAGE_HPP