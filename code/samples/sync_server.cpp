#include "api.h"

using namespace machyAPI;

int main()
{
    int err = machysockets_Sync::create_passive_synchronous_socket();
    if(err!=0)
    {
        std::cout<<"error creating passive synchronous socket: "<< err <<std::endl;
        exit(-1);
    }
    else
    {
        std::cout<<"succesfully connected to synchrounous client socket!"<<std::endl;
    }
}