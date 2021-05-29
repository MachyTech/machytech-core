#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <string.h>

#include "machy_utils.h"

#define WEBURL_SIZE 254

struct Data{
    float x, y;
};

struct Sim{
    float x, y, t, v, theta;
};

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

void read_remote_csv(char* weburl, std::vector<Data> &position);
/* 
 * reads a remote csv file and stores the data points on the GPU
 * @param weburl: url to datapoints
 * @param position: position data
 */

void read_remote_csv(char* weburl, std::vector<Sim> &virposition);
/* 
 * @overload read_remote_csv(), reads a remote csv file and stores the data points on the GPU
 * @param weburl: url to datapoints
 * @param position: use sim data for position
 */

void print_csv(std::vector<Data> &position);
void print_csv(std::vector<Sim> &virposition);

namespace MachyCore
{
    /*
     *
     */
    class Scene
    {
        protected:
            GLuint program, n_points;
            std::vector <Data> data;
            char* weburl;
        public:
            Scene(GLuint a, std::string b)
            {
                program = a;
                weburl = (char*)malloc(b.length() * sizeof( char ));
                strcpy(weburl, b.c_str());
            }
            virtual void bind_buffer() {};
            virtual void print_data() {};
            virtual void render(GLFWwindow* win) {};
            virtual void render(GLFWwindow* win, int linewidth, int samplesize) {};
            virtual ~Scene() {};
    };

    class RobotPathSim : public Scene
    {
        private:
            GLuint vertex_array_object, buffer, rot_location, vpos_location, off_location, len_location;
            std::vector <Sim> simdata;
        public:
            RobotPathSim (GLuint a, std::string b) : Scene(a, b)
            {
                glCreateVertexArrays(1, &vertex_array_object);
                glBindVertexArray(vertex_array_object);

                rot_location = glGetUniformLocation(program, "ROT");
                off_location = glGetUniformLocation(program, "OFF");
                vpos_location = glGetAttribLocation(program, "position");
                len_location = glGetUniformLocation(program, "LEN");

                glGenBuffers(1, &buffer);
                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                clock_t begin_t = clock();
                read_remote_csv(weburl, simdata);
                clock_t end_t = clock();
                printf("formatted data in : %lf\n\n", double(end_t-begin_t)/double(CLOCKS_PER_SEC));
                // print_csv(simdata);
                n_points = simdata.size();
                bind_buffer();
            }
            void bind_buffer();
            void render(GLFWwindow* win, int linewidth, GLint samplesize);
            void print_data();
            ~RobotPathSim()
            {
                glDeleteVertexArrays(1, &vertex_array_object);
                glDeleteProgram(program);
                glDeleteVertexArrays(1, &vertex_array_object);
            }
    };
}
#endif