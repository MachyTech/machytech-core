#include "machy_utils.h"

namespace MachyCore
{
    /* callbacks */


    void Environment::appendVariable(Variables *variable){
        variables->push_back ( variable );
    }

    void Environment::print()
    {
        std::vector<Variables*>::iterator it;
        
        for (it = variables->begin (); it < variables->end (); it++)
        {
            std::string str = (*it)->get_var();
            std::cout<<str<<std::endl;
        }
    }

    std::string Environment::get(int varNum)
    {
        std::vector<Variables*>::iterator it = variables->begin () + varNum;
        return (*it)->get_var();
    }

    std::string MachyGLutils::read_shader(std::string direction)
    {
        std::ifstream in(direction);
        std::string contents((std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());
        std::string shader_text = contents.c_str();
        return shader_text;
    }

    GLuint MachyGLutils::link_shader(std::string vs_direction, std::string fs_direction)
    {
        std::cout<<"linking shaders to program"<<std::endl;
        vertex_shader_text = read_shader(vs_direction);
        vs_text = vertex_shader_text.c_str();
        std::cout<<"using vertex shader in: \n"<<vs_direction<<std::endl;
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vs_text, NULL);
        glCompileShader(vertex_shader);
        vertex_flag = get_compile_data(vertex_shader);
        if (vertex_flag==1){
            std::cout<<"Error when compiling"<<std::endl;
            throw;
        }
        std::cout<<"using vertex shader : \n"<<vs_text<<std::endl;

        fragment_shader_text = read_shader(fs_direction);
        fs_text = fragment_shader_text.c_str();

        std::cout<<"using the fragment shader in : \n"<<fs_direction<<std::endl;

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fs_text, NULL);
        glCompileShader(fragment_shader);

        fragment_flag = get_compile_data(fragment_shader);
        if (fragment_flag==1){
            std::cout<<"Error when compile the fragment shader"<<std::endl;
            throw;
        }

        std::cout<<"using the fragment shader : \n"<<fs_text<<std::endl;

        GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        std::cout<<"linking succesfull"<<std::endl;
        return program;
    }

    int MachyGLutils::get_compile_data(GLuint shader)
    {
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            /* ERROR handling */
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
            /* make sure we don't leak the shader */
            glDeleteShader(shader);
            for(int i=0; i<maxLength; i++){
                std::cout<<errorLog[i];
            }
            /* return 1 on error */
            return 1;
        }
        /* return 0 when succesfull */
        return 0;
    }
}
