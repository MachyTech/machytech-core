#include "machygl.h"
#include "api.h"
#include "machyscene.h"

int main()
{
    machygl::utils glutils;

    machygl::Window win("640", "360");
    GLuint program = glutils.link_shader("shader/basic.vert","shader/basic.frag");
    int err = machyAPI::machysockets_aSync::asynchronous_server();

    machyscene::Scene *scene;
    machyscene::TrajectorySim robotpath(program);
    scene = &robotpath;
    while(1){
        scene->render(win.window, 10, 50);
    }
    return 0;
}