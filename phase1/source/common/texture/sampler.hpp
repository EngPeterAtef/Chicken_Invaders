#pragma once

#include <glad/gl.h>
#include <json/json.hpp>
#include <glm/vec4.hpp>

namespace our
{

    // This class defined an OpenGL sampler
    class Sampler
    {
        // The OpenGL object name of this sampler
        GLuint name;

    public:
        // This constructor creates an OpenGL sampler and saves its object name in the member variable "name"
        Sampler()
        {
            // TODO: (Req 6) Complete this function
            // this function generates one sampler object (first parameter is the count)
            // second parameter is unsigned integer where the name of the generated
            // sampler is stored
            glGenSamplers(1, &name);
        };

        // This deconstructor deletes the underlying OpenGL sampler
        ~Sampler()
        {
            // TODO: (Req 6) Complete this function
            // this function delete one sampler object (first parameter is the count)
            // second parameter is unsigned integer where the name of the sampler to delete is stored
            glDeleteSamplers(1, &name);
        }

        // This method binds this sampler to the given texture unit
        /**
         * @brief This method binds this sampler to the given texture unit
         *
         * @param textureUnit GLuint the texture to bind the sampler to it
         */
        void bind(GLuint textureUnit) const
        {
            // TODO: (Req 6) Complete this function
            // this function bind a sampler object whose name is the second prameter(name)
            // to a texture unit (first parameter)
            glBindSampler(textureUnit, name);
        }

        // This static method ensures that no sampler is bound to the given texture unit
        /**
         * @brief This static method ensures that no sampler is bound to the given texture unit
         *
         * @param textureUnit GLuint the texture to inbind all bound smapler to it
         */
        static void unbind(GLuint textureUnit)
        {
            // TODO: (Req 6) Complete this function
            // this function takes 1st parameter the texture unit and the second parameter is zero
            // so it unbinds all sampler bound previously to it
            glBindSampler(textureUnit, 0);
        }

        // This function sets a sampler paramter where the value is of type "GLint"
        // This can be used to set the filtering and wrapping parameters
        /**
         * @brief  This function sets a sampler paramter where the value is of type "GLint"
         *
         * @param parameter GLenum which parameter of the sampler should be modified
         * @param value GLint the new value of the specified sampler parameter
         */
        void set(GLenum parameter, GLint value) const
        {
            // TODO: (Req 6) Complete this function
            // name is the name of the sampler object whose parameter should be modified
            glSamplerParameteri(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat"
        // This can be used to set the "GL_TEXTURE_MAX_ANISOTROPY_EXT" parameter
        /**
         * @brief This function sets a sampler paramter where the value is of type "GLfloat"
         *
         * @param parameter GLenum which parameter of the sampler should be modified
         * @param value GLint the new value of the specified sampler parameter
         */
        void set(GLenum parameter, GLfloat value) const
        {
            // TODO: (Req 6) Complete this function
            // name is the name of the sampler object whose parameter should be modified
            glSamplerParameterf(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat[4]"
        // This can be used to set the "GL_TEXTURE_BORDER_COLOR" parameter

        void set(GLenum parameter, glm::vec4 value) const
        {
            // 1st parameter is the name of the samplet objct to modify
            // 2nd is an enumeration value that specifies which parameter to modify
            // 3rd is is a pointer to the first component of a vec4 value that represents the new value of the specified sampler parameter
            glSamplerParameterfv(name, parameter, &(value.r));
        }

        // Given a json object, this function deserializes the sampler state
        /**
         * @brief Given a json object, this function deserializes the sampler state
         *
         * @param data json object
         */
        void deserialize(const nlohmann::json &data);

        Sampler(const Sampler &) = delete;
        Sampler &operator=(const Sampler &) = delete;
    };

}