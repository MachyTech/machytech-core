#include "machygl.h"
#include "api.h"
#include "machyscene.h"

int main()
{
    machygl::utils glutils;

    machygl::Window win("640", "360");
    GLuint program = glutils.link_shader("shader/basic.vert", "shader/basic.frag");
    // create a thread pool
    unsigned int thread_pool_size = std::thread::hardware_concurrency() * 2;
    if (thread_pool_size == 0)
        thread_pool_size = DEFAULT_THREAD_POOL_SIZE;

    // start a server
    unsigned short port_num = 3333;
    machyAPI::machysockets_aSync::server srv;

    srv.start(port_num, thread_pool_size);

    // wait for api (no busy wait?)
    std::unique_lock<std::mutex> lk(machycore::m_machydata);
    machycore::render_ready.wait(lk, []{return machycore::load_scene;});

    // create scene
    machyscene::Scene *scene;
    machyscene::TrajectorySim robotpath(program);
    scene = &robotpath;

    // let api know we succesfully loaded the scene
    machycore::scene_loaded = true;
    lk.unlock();
    machycore::render_ready.notify_one();

    while(1){
        scene->render(win.window, 10, 50);
    }
    return 0;
}