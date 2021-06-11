#include "machyscene.h"
#include "machygl.h"
#include "api.h"

int main()
{
    machygl::utils glutils;
    machygl::Window win("640", "360");
    GLuint program = glutils.link_shader("shader/basic.frag","shader/basic.vert");
    int err = machyAPI::machysockets_aSync::asynchronous_server();
    machyscene::Scene *scene;
    scene = new machyscene::trajectorysim(program);
    while(1){
        scene->render(win.window, 10, 5);
    }
    return 0;
}