#include "api.h"

using namespace machyAPI;

int main()
{
    int err = machysockets_aSync::asynchronous_httpserver();
    if(err != 0)
    {
        std::cout<<"error in asynchronous http server"<<err<<std::endl;
        exit(-1);
    }
    else
    {
        std::cout<<"succesful session\n";
    }
}