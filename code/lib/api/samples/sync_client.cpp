#include "api.h"

using namespace machyAPI;

int main()
{
    int err = machysockets_Sync::synchronous_client();
    if(err!=0)
    {
        std::cout<<"error connecting to server"<<std::endl;
        exit(-1);
    }
    else
    {
        std::cout<<"succesfully connected\n";
    }
}