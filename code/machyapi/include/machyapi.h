#ifndef MACHYAPI_H
#define MACHYAPI_H

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <fstream>
#include <string>

struct Data{
    float x, y;
};

struct Sim{
    float x, y, t, v, theta;
}

namespace MachyAPI
{
    /*
     * API machytech
     */
     class MachyData
     {
         public:
            void read_csv(std::string filedir, std::vector<Data> &position);
            /* 
            * this function reads a csv file locally and stores the data points on the GPU
            * @param filedir: path to local file
            * @param position: position data 
            */
            void read_csv(std::string filedir, std::vector<Sim> &virposition);
            /*
            * @overload void read_csv(), this function reads a csv file locally and stores the data points on the GPU
            * @param filedir: path to local file
            * @param virposition: use sim data as position
            */
            void print_csv(std::vector<Data> &position);
            /*
             * this function prints data structure
             * @param position: data structure with position data
             */
            void print_csv(std::vector<Sim> &virposition);
            /*
             * this function prints a data structure
             * @param position: data structure with virtual position data
             */
            
     }
}