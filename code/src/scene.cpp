#include "scene.h"

namespace MachyCore
{
    void RobotPathSim::bind_buffer()
    {
        glBufferData(GL_ARRAY_BUFFER, simdata.size()*sizeof(simdata), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, simdata.size()*sizeof(simdata), &simdata[0]);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(simdata[0]), NULL);
        glEnableVertexAttribArray(vpos_location);
    }

    void RobotPathSim::render(GLFWwindow* win)
    {
        for (int i=0; i<simdata.size(); i++)
        {
            double t_begin = simdata[i].t;
            clock_t ct_begin = clock();

            const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
            glClearBufferfv(GL_COLOR, 0, color);

            int width, height;
            glfwGetFramebufferSize(win, &width, &height);
            glViewport(0, 0, width, height);
            glUseProgram(program);

            
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, -(float)simdata[i].theta, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::vec2 offset = glm::vec2(simdata[i].x, simdata[i].y);

            glUniform2fv(off_location, 1, glm::value_ptr(offset));
            glUniformMatrix4fv(rot_location, 1, GL_FALSE, glm::value_ptr(trans));
            glDrawArrays(GL_LINE_STRIP, 0 , n_points);

            double t_end = simdata[i+1].t;
            clock_t ct_end = clock();
            double cdt = double(ct_end-ct_begin)/double(CLOCKS_PER_SEC);
            double dt = double(t_end - t_begin);

            printf("\rfps : %lf", 1/cdt);
            // printf(" idle: %lf", double(dt-cdt));
            if(double(dt-cdt)>0){usleep(double(dt-cdt) * 1000000);}

            glfwSwapBuffers(win);
            glfwPollEvents();
            if (glfwWindowShouldClose(win)){
                exit(1);
            }
        }
    }

    void RobotPathSim::print_data()
    {
        for (const auto &arr: simdata){
            std::cout<<arr.x<<", "<<arr.y<<", "<<arr.t<<", "<<arr.v<<", "<<arr.theta<<std::endl;
        }
    }
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size*nmemb);
    return size * nmemb;
}

void read_remote_csv(char* weburl, std::vector<Data> &position)
{
    CURL *curl_handle;
    CURLcode res;
    std::string buffer;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    if(curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_URL, weburl);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl_handle);
        std::stringstream ssline(buffer);
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else{
            std::string line;
            float value[7];
            while(std::getline(ssline, line, '\n'))
            {
                std::stringstream ss(line);
                ss >> value[0];
                ss.ignore();
                ss >> value[1];
                position.push_back({value[0], value[1]});
            }
        }
    }
}


void read_remote_csv(char* weburl, std::vector<Sim> &virposition)
{
    CURL *curl_handle;
    CURLcode res;
    std::string buffer;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    if(curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_URL, weburl);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl_handle);
        std::stringstream ssline(buffer);
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            exit(-1);
        }
        else{
            std::string line;
            float value[7];
            while(std::getline(ssline, line, '\n'))
            {
                std::stringstream ss(line);
                for(int i=0; i<7; i++)
                {
                    ss >> value[i];
                    ss.ignore();
                }
                virposition.push_back({value[0], value[1], value[2], value[3], value[6]});
            }
        }
    }
}

void read_csv(std::string filedir, std::vector<Sim> &virposition)
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
            virposition.push_back({value[0], value[1], value[2], value[3], value[6]});
        }
    }
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

void read_csv(std::string filedir, std::vector<Data> &position)
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
