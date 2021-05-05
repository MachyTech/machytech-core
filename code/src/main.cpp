#include "socketdev.h"
#include "machy_utils.h"
#include "scene.h"
#include "envs.h"

//#define DEBUG

std::vector<MachyCore::Variables*> *variables;

void create_env(MachyCore::Environment *env){
    env->appendVariable( new MachyCore::FragShader("GLSL_APP_VERT", "shader/basic.vert"));
    env->appendVariable( new MachyCore::VertexShader("GLSL_APP_FRAG", "shader/basic.frag"));
    env->appendVariable( new MachyCore::IPaddress("IP_ADDR", "127.0.0.1"));
    env->appendVariable( new MachyCore::Portaddress("PORT_ADDR", "8000"));
}

int main()
{
    char *buffer_pointer;
    /* creating the environment */
    MachyCore::Environment *env = new MachyCore::Environment ();
    create_env(env);
    env->print();
    /* initialize the machy util functions */
    MachyCore::MachyGLutils glutils;
    /* creating openGL context */
    MachyCore::Window win;
    /* call shader linker */
    GLuint program = glutils.link_shader(env->get(0), env->get(1));
    MachyCore::Scene *scene;
    MachyCore::RobotPathSim robotpath(program);
    scene = &robotpath;
    while(1){
        scene->render(win.window);
    }
    return 0;
}