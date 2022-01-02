#include "api.h"

using namespace machyAPI;

int main()
{
    int err = machysockets_aSync::asynchronous_server("3333");
    if(err!=0)
    {
        std::cout<<"error in asynchronous server: "<<err<<std::endl;
        exit(-1);
    }
    else
    {
        std::cout<<"succesfully connected to synchrounous server socket!\n";
    }
}