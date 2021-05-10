#ifndef _ENVS_H_
#define _ENVS_H_

#include <stdlib.h>
#include <string>
#include <iostream>

#define GLSL_APP_FRAG 0
#define GLSL_APP_VERT 1
#define SCENE 2
#define TCP_IP 3
#define TCP_PORT 4
#define HTTP_IP 5
#define HTTP_PORT 6
#define HTTP_ROUTE 7
#define CURL_WEBURL 8
#define SAMPLE_SIZE 9
#define LINEWIDTH 10

namespace MachyCore
{
    class Variables
    {
        protected:
            const char* string_var;
            const char* default_str;
        public:
            Variables( const char* a, const char* b ){ 
                string_var = a;
                default_str = b;
            }
            virtual const char* get_var(){ return 0;};
            virtual void print() {};
            virtual ~Variables() {};
    };

    class StdEnvVariable : public Variables
    {
        public:
            StdEnvVariable( const char* a, const char* b) : Variables( a,b ) {};
            const char* get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str; }
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s: %s\n", string_var, getenv(string_var)); }
                else { printf("%s:%s\n", string_var, default_str); }
            }
    };

    class FragShader : public Variables
    {
        public:
            FragShader( const char* a, const char* b) : Variables( a, b ) {};
            const char* get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var);}
                else { return default_str; }
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };

    class VertexShader : public Variables
    {
        public:
            VertexShader( const char* a, const char* b) : Variables( a, b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var);}
                else { return default_str;}
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };

    class TCPIPaddress : public Variables
    {
        public:
            TCPIPaddress( const char* a, const char* b) : Variables( a, b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str;}
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };

    class TCPPortaddress : public Variables
    {
        public:
            TCPPortaddress( const char* a, const char* b) : Variables( a, b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str; }
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };

    class CURLWeburl: public Variables
    {
        public:
            CURLWeburl( const char* a, const char* b) : Variables( a, b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str;}
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };

    class HTTPIPaddress: public Variables
    {
        public:
            HTTPIPaddress( const char* a, const char* b) : Variables( a,b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str;}
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };

    class HTTPPortaddress: public Variables
    {
        public:
            HTTPPortaddress( const char* a, const char* b) : Variables( a,b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str;}
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };

    class HTTPRoutename: public Variables
    {
        public:
            HTTPRoutename( const char* a, const char* b) : Variables( a,b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str;}
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };

    class SelectScene: public Variables
    {
        public:
            SelectScene( const char* a, const char* b) : Variables( a,b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str;}
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };

    class SampleSize: public Variables
    {
        public:
            SampleSize( const char* a, const char* b) : Variables( a,b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str;}
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };

    class LineWidth: public Variables
    {
        public:
            LineWidth( const char* a, const char* b) : Variables( a,b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str;}
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s, %s\n",string_var, getenv(string_var));}
                else { printf("%s, %s\n", string_var, default_str);}
            }
    };
}
#endif /* _ENVS_H_ */