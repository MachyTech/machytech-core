#include "api.h"

using namespace machyAPI;

int main()
{
    int err = machysockets::connect_synchronous_socket();
    if(err!=0)
    {
        std::cout<<"error in connect_synchronous_socket: "<< err <<std::endl;
        exit(-1);
    }
    else
    {
        std::cout<<"succesfully connected to synchrounous server socket!\n"
        <<"sending data..."<<std::endl;
    }
}