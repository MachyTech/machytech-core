#include "machygl.h"
#include "api.h"
#include "machyscene.h"

int main()
{
    std::cout<<"starting glutils"<<std::endl;
    machygl::utils glutils;
    std::cout<<"opening window"<<std::endl;
    machygl::Window win("640", "360");
    std::cout<<"succesfully opened window"<<std::endl;
    GLuint program = glutils.link_shader("shader/basic.vert","shader/basic.frag");
    std::cout<<"waiting 60 seconds for trajectory"<<std::endl;
    int err = machyAPI::machysockets_aSync::asynchronous_server();
    std::cout<<"starting trajectory sim"<<std::endl;
    machyscene::Scene *scene;
    machyscene::TrajectorySim robotpath(program);
    scene = &robotpath;
    while(1){
        scene->render(win.window, 10, 50);
    }
    return 0;
}