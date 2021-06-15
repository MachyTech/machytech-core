#ifndef MACHYCORE_H
#define MACHYCORE_H

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <vector>
#include <iostream>

namespace machycore
{
    /*
     * machycore includes function that connect all the piecec
     * of software together
     */
    struct Data{
        float x, y;
        Data(float a, float b) : x(a), y(b)
        {} 
    };
    /*
     * data structure with x and y data accepted by the openGL context
     */
    extern std::vector<Data*> *position;

    struct Sim{
        float x, y, t, v, theta;
        Sim(float a[5]) : x(a[0]),
            y(a[1]), t(a[2]), v(a[3]), theta(a[4])
        {}
    };
    /*
     * data structure with x and y data. Also it uses simulation
     * data available from the path trajectory.
     */
    struct Square
    {
        float x, y;
    };

    /*
     * sample triangle
     */
    extern std::vector<Sim*> *virposition;
    
    void print_pos_data();
    /*
     * print data structures from machycore
     */
    void print_vpos_data();
    /*
     * overload function print virtual data structure
     * from machytech core
     */
}
#endif