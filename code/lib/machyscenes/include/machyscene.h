#ifndef TRAJECTORY_SCENE_H
#define TRAJECTORY_SCENE_H

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
#include <string>

#include <machycore.h>
#include <machygl.h>
#include <api.h>

namespace machyscene
{
    /*
     * machyscenes
     */
    class Scene
    {
        public:
            Scene(GLuint a)
            {
                program = a;
            }
            virtual void bind_buffer() {};
            virtual void render(GLFWwindow* win, int linewidth, int samplesize) {};
            virtual ~Scene() {};
        protected:
            GLuint program, n_points;
    };

    class trajectorysim : public Scene
    {
        public:
            trajectorysim ( GLuint a ) : Scene(a)
            {
                glCreateVertexArrays(1, &vertex_array_object);
                glBindVertexArray(vertex_array_object);

                rot_location = glGetUniformLocation(program, "ROT");
                off_location = glGetUniformLocation(program, "OFF");
                vpos_location = glGetAttribLocation(program, "position");
                len_location = glGetUniformLocation(program, "LEN");

                glGenBuffers(1, &buffer);
                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                machycore::print_vpos_data();
            }
            void bind_buffer();
            void render(GLFWwindow* win, int linewidth, int samplesize);
            void print_data();
            ~trajectorysim()
            {
                glDeleteVertexArrays(1, &vertex_array_object);
                glDeleteProgram(program);
            }
        private:
            GLuint vertex_array_object, buffer, rot_location, vpos_location, off_location, len_location;       
    };
}

#endif