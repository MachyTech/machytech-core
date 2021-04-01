#ifndef _ENVS_H_
#define _ENVS_H_

#include <stdlib.h>
#include <string>
#include <iostream>

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
                if (getenv(string_var)!=NULL){ printf("%s\n", getenv(string_var));}
                else { printf("%s\n", default_str);}
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
                if (getenv(string_var)!=NULL){ printf("%s\n", getenv(string_var));}
                else { printf("%s\n", default_str);}
            }
    };

    class IPaddress : public Variables
    {
        public:
            IPaddress( const char* a, const char* b) : Variables( a, b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str;}
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s\n", getenv(string_var));}
                else { printf("%s\n", default_str);}
            }
    };

    class Portaddress : public Variables
    {
        public:
            Portaddress( const char* a, const char* b) : Variables( a, b ) {};
            const char *get_var()
            {
                if (getenv(string_var)!=NULL){ return getenv(string_var); }
                else { return default_str; }
            }
            void print(){
                if (getenv(string_var)!=NULL){ printf("%s\n", getenv(string_var));}
                else { printf("%s\n", default_str);}
            }
    };
}
#endif /* _ENVS_H_ */