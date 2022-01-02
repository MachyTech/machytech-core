#include "socketdev.h"
#include "machy_utils.h"
#include "scene.h"
#include "envs.h"

//#define DEBUG

std::vector<MachyCore::Variables*> *variables;

void create_env(MachyCore::Environment *env){
    env->appendVariable( new MachyCore::StdEnvVariable("GLSL_APP_FRAG", "shader/basic.frag"));
    env->appendVariable( new MachyCore::StdEnvVariable("GLSL_APP_VERT", "shader/basic.vert"));
    env->appendVariable( new MachyCore::StdEnvVariable("SCENE","robotpath"));
    env->appendVariable( new MachyCore::StdEnvVariable("TCP_IP", "127.0.0.1"));
    env->appendVariable( new MachyCore::StdEnvVariable("TCP_PORT", "3000"));
    env->appendVariable( new MachyCore::StdEnvVariable("HTTP_IP", "127.0.0.1"));
    env->appendVariable( new MachyCore::StdEnvVariable("HTTP_PORT", "3015"));
    env->appendVariable( new MachyCore::StdEnvVariable("HTTP_ROUTE","/default"));
    env->appendVariable( new MachyCore::StdEnvVariable("CURL_WEBURL", "http://0.0.0.0:8000/trajectory_100_fpg_out.txt"));
    env->appendVariable( new MachyCore::StdEnvVariable("SAMPLE_SIZE", "5"));
    env->appendVariable( new MachyCore::StdEnvVariable("LINEWIDTH", "10"));
    env->appendVariable( new MachyCore::StdEnvVariable("WINDOW_WIDTH", "640"));
    env->appendVariable( new MachyCore::StdEnvVariable("WINDOW_HEIGHT", "360"));
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
    MachyCore::Window win(env->get(WINDOW_WIDTH), env->get(WINDOW_HEIGHT));
    /* call shader linker */
    GLuint program = glutils.link_shader(env->get(GLSL_APP_VERT), env->get(GLSL_APP_FRAG));
    MachyCore::Scene *scene;
    MachyCore::RobotPathSim robotpath(program, env->get(CURL_WEBURL));
    if(env->get(SCENE).compare("robotpath") == 0){
        std::cout<<"scene : robotpath"<<std::endl;
        scene = &robotpath;
    }
    while(1){
        scene->render(win.window, std::stoi (env->get(LINEWIDTH)), std::stoi (env->get(SAMPLE_SIZE)));
    }
    return 0;
}