#include "machyscene.h"

namespace machyscene
{
    void TrajectorySim::bind_buffer()
    {
        glBufferData(GL_ARRAY_BUFFER, machycore::virposition->size()*sizeof(machycore::virposition), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, machycore::virposition->size()*sizeof(machycore::virposition), &machycore::virposition[0]);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(machycore::virposition[0]), NULL);
        glEnableVertexAttribArray(vpos_location);
    }

    void TrajectorySim::render(GLFWwindow* win, int linewidth, int samplesize)
    {
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
            glm::vec2 offset = glm::vec2(0.0, 0.0);

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

            //printf(":\rfps : %lf", 1/cdt);

            if(double(dt-cdt)>0){usleep(double(dt-cdt)*1000000);}
            
            glfwSwapBuffers(win);
            glfwPollEvents();
            if (glfwWindowShouldClose(win)){
                exit(1);
            }
        }
    }
    void TrajectorySim::print_buffer()
    {
        std::cout<<"t, v, theta\n";
        for (const auto &arr: *machycore::virposition){
            std::cout<<arr->t<<", "<<arr->v<<", "<<arr->theta<<std::endl;
        }
    }
/* 
    void PlotData::print_buffer()
    {
        std::cout<<"x, y, t, v, theta\n";
        for (const auto &arr: *machycore::trajectory){
            std::cout<<arr->x<<", "<<arr->y<<", "<<arr->t<<", "<<arr->v<<", "<<arr->theta<<std::endl;
        }
    } */

/*     void PlotData::print_buffer()
    {
        std::cout<<"printing buffer.... x, y\n";
        for (int i=0; i<(*machycore::virposition).size(); i++)
        {
            std::cout<<i<<" "<<&(*machycore::virposition)[i]->x<<" "<<(*machycore::virposition)[i]->x<<", "<<&(*machycore::virposition)[i]->y<<", "<<(*machycore::virposition)[i]->y<<std::endl;
        }
    } */

    void PlotData::print_buffer()
    {
        std::cout<<"printing buffer.... x, y\n";
        for (int i=0; i<((*machycore::trajectory).size()-2800); i++)
        {
            std::cout<<i<<" "<<&(*machycore::trajectory)[i]->x<<" "<<(*machycore::trajectory)[i]->x<<", "<<&(*machycore::trajectory)[i]->y<<", "<<(*machycore::trajectory)[i]->y<<std::endl;
        }
    }

    void PlotData::bind_buffer()
    {
        std::cout<<"-------------------------------buffer---------------------------\n";
        std::cout<<"array size : "<<(*machycore::trajectory).size()<<std::endl;
        std::cout<<"array size in bytes : "<<(*machycore::trajectory).size()*sizeof((*machycore::trajectory))<<std::endl;
        std::cout<<"pointer to first member of object : "<<&(*machycore::trajectory)[0]->x<<std::endl;
        std::cout<<"pointer to first object : "<<&(*machycore::trajectory)[0]<<std::endl;
        std::cout<<"byte size of array entry : "<<sizeof((*machycore::trajectory))<<std::endl;
        std::cout<<"byte size of local array entry : "<<sizeof(machycore::trajectory)<<std::endl;
        std::cout<<"byte size of first object : "<<sizeof((*machycore::trajectory)[0])<<std::endl;

        glBufferData(GL_ARRAY_BUFFER, (*machycore::trajectory).size()-1, &(*machycore::trajectory)[0]->x, GL_STATIC_DRAW);
        //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (*machycore::trajectory).size()*sizeof((*machycore::trajectory)), &(*machycore::trajectory)[0]->x);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, 2*sizeof((*machycore::trajectory)[0]), NULL);
        glEnableVertexAttribArray(vpos_location);
    }

/*     void PlotData::bind_buffer()
    {
        std::cout<<"-------------------------------buffer---------------------------\n";
        std::cout<<"array size of virtual position : "<<(*machycore::virposition).size()<<std::endl;
        std::cout<<"byte size of virtual position : "<<(*machycore::virposition).size()*sizeof((*machycore::virposition))<<std::endl;
        std::cout<<"pointer to first object : "<<&(*machycore::virposition)[0]<<std::endl;
        std::cout<<"byte size of array : "<<sizeof((*machycore::virposition))<<std::endl;
        std::cout<<"byte size of local array : "<<sizeof(machycore::virposition)<<std::endl;
        std::cout<<"byte size of first object : "<<sizeof((*machycore::virposition)[0]->x)<<std::endl;
        glBufferData(GL_ARRAY_BUFFER, (*machycore::virposition).size()*sizeof((*machycore::virposition)), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (*machycore::virposition).size()*sizeof((*machycore::virposition)), &(*machycore::virposition)[0]);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof((*machycore::virposition)[0]), NULL);
        glEnableVertexAttribArray(vpos_location);
    } */
    void PlotData::render(GLFWwindow* win)
    {
        const GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, color);

        int width, height;
        glfwGetFramebufferSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glUseProgram(program);
        
        //const auto &arr = *machycore::virposition;
        //glm::vec2 offset = glm::vec2(arr[0]->x, arr[0]->y);
        glm::vec2 offset = glm::vec2(0.0 , 0.0);
        //glm::vec2 offset = glm::vec2((*machycore::virposition)[0]->x, (*machycore::virposition)[0]->y);
        glUniform2fv(off_location, 1, glm::value_ptr(offset));
        glUniform1f(len_location, (GLfloat) 50/10);
        glDrawArrays(GL_LINE_STRIP, 0, n_points);
        glLineWidth(5.0);
        //glDrawArrays(GL_POINTS,0 ,n_points);
        glEnable(GL_LINE_SMOOTH);
        //glPointSize(5.0);

        glfwSwapBuffers(win);
        glfwPollEvents();
        if (glfwWindowShouldClose(win)){
            exit(1);
        }
    }
}