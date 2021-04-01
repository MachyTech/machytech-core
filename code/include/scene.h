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

struct Data{
    float x, y;
};

struct Sim{
    float x, y, t, v, theta;
};

void read_csv(std::string filedir, std::vector<Data> &position);
void read_csv(std::string filedir, std::vector<Sim> &virposition);
void read_remote_csv(char* weburl, std::vector<Data> &position);
void read_remote_csv(char* weburl, std::vector<Sim> &virposition);
void print_csv(std::vector<Data> &position);
void print_csv(std::vector<Sim> &virposition);

namespace MachyCore
{
    class Scene
    {
        protected:
            GLuint program, n_points;
            std::vector <Data> data;
        public:
            Scene(GLuint a)
            {
                program = a;
            }
            virtual void bind_buffer() {};
            virtual void print_data() {};
            virtual void render(GLFWwindow* win) {};
            virtual ~Scene() {};
    };

    class RobotPathSim : public Scene
    {
        private:
            GLuint vertex_array_object, buffer, rot_location, vpos_location, off_location;
            std::vector <Sim> simdata;
        public:
            RobotPathSim (GLuint a) : Scene(a)
            {
                glCreateVertexArrays(1, &vertex_array_object);
                glBindVertexArray(vertex_array_object);

                rot_location = glGetUniformLocation(program, "ROT");
                off_location = glGetUniformLocation(program, "OFF");
                vpos_location = glGetAttribLocation(program, "position");

                glGenBuffers(1, &buffer);
                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                clock_t begin_t = clock();
                read_remote_csv("http://0.0.0.0:8000/trajectory_100_fpg_out.txt", simdata);
                clock_t end_t = clock();
                printf("formatted data in : %lf\n\n", double(end_t-begin_t)/double(CLOCKS_PER_SEC));
                n_points = simdata.size();
                bind_buffer();
            }
            void bind_buffer();
            void render(GLFWwindow* win);
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