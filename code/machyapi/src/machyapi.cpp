#include "machyapi.h"

namespace MachyAPI
{
    void MachyData::read_csv(std::string filedir, std::vector<Sim> &virposition)
    {
        std::ifstream file(filedir);
        clock_t begin_t = clock();
        if(!file.is_open()) throw std::runtime_error("could not open file");

        if(file.good())
        {
            std::string line;
            float value[7];
            while(std::getline(file, line))
            {
                std::stringstream ss(line);
                for(int i=0; i<7; i++)
                {
                    ss >> value[i];
                    ss.ignore();
                }
                virposition.push_back({value[0], value[1], value[2], value[3], (value[6]+1.57)});
            }
        }
    }

    void MachyData::read_csv(std::string filedir, std::vector<Data> &position)
    {
        std::ifstream file(filedir);
        clock_t begin_t = clock();

        if(!file.is_open()) throw std::runtime_error("could not open file");

        if(file.good())
        {  
            std::string line;
            float value[2];
            while(std::getline(file, line))
            {
                std::stringstream ss(line);
                ss >> value[0];
                ss.ignore();
                ss >> value[1];
                position.push_back({value[0], value[1]});
            }
        }
        clock_t end_t = clock();
        printf("read file in %lf\n", double(end_t-begin_t)/double(CLOCKS_PER_SEC));
    }

    void print_csv(std::vector<Data> &position)
    {
        for (const auto &arr: position){
            std::cout<< arr.x<< ", " << arr.y <<std::endl;
        }
    }

    void print_csv(std::vector<Sim> &virposition)
    {
        std::cout<<"x, y, t, v, theta"<<std::endl;
        for (const auto &arr: virposition){
            std::cout<<arr.x<<". "<<arr.y<<", "<<arr.t<<", "<<arr.v<<", "<<arr.theta<<std::endl;
        }
    }
}