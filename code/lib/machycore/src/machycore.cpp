#include "machycore.h"

namespace machycore
{
    std::vector<Data*> *position =  new std::vector<Data*>;    
    /* 
     * position vector pointer
     */
    std::vector<Sim*> *virposition = new std::vector<Sim*>;
    /*
     * virtual position pointer
     */
    void print_pos_data()
    {
        std::cout<<"x, y, t, v, theta\n";
        for (const auto &arr: *position){
            std::cout<< arr->x<< ", " << arr->y <<std::endl;
        }
    }

    void print_vpos_data()
    {
        std::cout<<"x, y, t, v, theta"<<std::endl;
        for (const auto &arr: *virposition){
            std::cout<<arr->x<<". "<<arr->y<<", "<<arr->t<<", "<<arr->v<<", "<<arr->theta<<std::endl;
        }
    }
}