#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
// using namespace std;
// #include <iostream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our
{

    class ShaderProgram
    {

    private:
        // Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram()
        {
            // TODO: (Req 1) Create A shader program
            // DONE
            program = glCreateProgram();
        }
        ~ShaderProgram()
        {
            // TODO: (Req 1) Delete a shader program
            // DONE
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use()
        {
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name)
        {
            // TODO: (Req 1) Return the location of the uniform with the given name
            // DONE
            return glGetUniformLocation(program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value)
        {
            // TODO: (Req 1) Send the given float value to the given uniform
            // DONE
            GLuint loc = getUniformLocation(uniform);
            glUniform1f(loc, value);
        }

        void set(const std::string &uniform, GLuint value)
        {
            // TODO: (Req 1) Send the given unsigned integer value to the given uniform
            // DONE
            GLuint loc = getUniformLocation(uniform);
            glUniform1ui(loc, value);
        }

        void set(const std::string &uniform, GLint value)
        {
            // TODO: (Req 1) Send the given integer value to the given uniform
            // DONE
            GLuint loc = getUniformLocation(uniform);
            glUniform1i(loc, value);
        }

        void set(const std::string &uniform, glm::vec2 value)
        {
            // TODO: (Req 1) Send the given 2D vector value to the given uniform
            // DONE
            GLuint loc = getUniformLocation(uniform);
            glUniform2fv(loc, 1, &value.x);
        }

        void set(const std::string &uniform, glm::vec3 value)
        {
            // TODO: (Req 1) Send the given 3D vector value to the given uniform
            // DONE
            GLuint loc = getUniformLocation(uniform);
            glUniform3fv(loc, 1, &value.x);
        }

        void set(const std::string &uniform, glm::vec4 value)
        {
            // TODO: (Req 1) Send the given 4D vector value to the given uniform
            // DONE
            GLuint loc = getUniformLocation(uniform);
            glUniform4fv(loc, 1, &value.x);
        }

        void set(const std::string &uniform, glm::mat4 matrix)
        {
            // TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            // DONE
            GLuint loc = getUniformLocation(uniform);
            glUniformMatrix4fv(loc, 1, false, &matrix[0][0]);
        }

        // TODO: (Req 1) Delete the copy constructor and assignment operator.
        // Question: Why do we delete the copy constructor and assignment operator?
        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
        /*
        Reason:
            We delete the copy constructor and assignment operator when we want to prevent copying of an object of a class
            This way, if someone tries to copy an object of that class, they will get a compile-time error.
            we need to prevent copying of an object of a class to acheive RAII (Resource Acquisition Is Initialization) so that
            the resource will be acquired and initialized in the constructor and released in the destructor.    
        */
    };

}

#endif