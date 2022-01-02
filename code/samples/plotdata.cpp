#include "machygl.h"
#include "api.h"
#include "machyscene.h"

int main()
{
    machygl::utils glutils;

    machygl::Window win("640", "360");
    GLuint program = glutils.link_shader("shader/plot.vert","shader/plot.frag");
    int err = machyAPI::machysockets_aSync::asynchronous_server();

    machyscene::Scene *scene;
    machyscene::PlotData robotpath(program);
    scene = &robotpath;
    while(1){
        scene->render(win.window);
    }
    return 0;
}