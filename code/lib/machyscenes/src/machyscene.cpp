#include "machyscene.h"

namespace machyscene
{
    void trajectorysim::bind_buffer()
    {
        glBufferData(GL_ARRAY_BUFFER, machycore::virposition->size()*sizeof(machycore::virposition), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, machycore::virposition->size()*sizeof(machycore::virposition), &machycore::virposition[0]);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(machycore::virposition[0]), NULL);
        glEnableVertexAttribArray(vpos_location);
    }
    void trajectorysim::render(GLFWwindow* win, int linewidth, int samplesize)
    {
/*         for(int i=0; i<machycore::virposition->size(); i++)
        { */
        double_t t_last = 0.0;
        clock_t ct_last = clock();
        for (const auto &arr: *machycore::virposition)
        {
            double_t t_begin = arr->t;
            clock_t ct_begin = clock();
            const GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glClearBufferfv(GL_COLOR, 0, color);
            int width, height;
            glfwGetFramebufferSize(win, &width, &height);
            glViewport(0, 0, width, height);
            glUseProgram(program);

            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, -(float)arr->t, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::vec2 offset = glm::vec2(arr->x, arr->y);

            glUniform2fv(off_location, 1, glm::value_ptr(offset));
            glUniformMatrix4fv(rot_location, 1, GL_FALSE, glm::value_ptr(trans));
            glUniform1f(len_location, (GLfloat) samplesize/10);
            glDrawArrays(GL_LINE_STRIP, 0 , machycore::virposition->size());
            glEnable(GL_LINE_SMOOTH);
            glLineWidth(linewidth);

            double dt = double(t_last - t_begin);
            t_last = arr->t;
            
            clock_t ct_last = clock();
            double cdt = double(ct_last-ct_begin)/double(CLOCKS_PER_SEC);
            ct_last = clock();

            printf(":\rfps : %lf", 1/cdt);

            if(double(dt-cdt)>0){usleep(double(dt-cdt)*1000000);}
            
            glfwSwapBuffers(win);
            glfwPollEvents();
            if (glfwWindowShouldClose(win)){
                exit(1);
            }
        }
    }
}