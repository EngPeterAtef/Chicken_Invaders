#pragma once

#include <glad/gl.h>

namespace our
{

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D
    {
        // The OpenGL object name of this texture
        GLuint name = 0;

    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name"
        Texture2D()
        {
            // TODO: (Req 5) Complete this function
            // generate one texture related to the name object
            glGenTextures(1, &name);
            // bind the texture to the GL_TEXTURE_2D
            glBindTexture(GL_TEXTURE_2D, name);
            // set the texture parameters
            //  GL_TEXTURE_WRAP_S: set the texture wrap parameter for texture coordinate s(X-axis)
            //  GL_TEXTURE_WRAP_T: set the texture wrap parameter for texture coordinate t(Y-axis)
            //  GL_REPEAT: repeat the texture image
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // GL_TEXTURE_MIN_FILTER: set texture minifying function
            // GL_TEXTURE_MAG_FILTER: set texture magnification function
            // GL_LINEAR: linear interpolation
            // GL_LINEAR_MIPMAP_NEAREST: PICK NEAREST MIP LEVEL THEN FILTER PIXELS LINEARLY
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            /*
            GL_LINEAR_MIP_LINEAR IS THE BEST (AND MOST
            EXPENSIVE) OPTION FOR REALISM. COMMONLY CALLED“TRILINEAR FILTERING”
            */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D()
        {
            // TODO: (Req 5) Complete this function
            /*
            - The first parameter (`1`) specifies the number of texture objects to be deleted. In this case, only one texture object is being deleted.
            - The second parameter (`&name`) is a pointer to an array of unsigned integer values representing the names of the texture objects to be deleted.
             In this case, the array contains only one element (`name`) which represents the name of the texture object to be deleted.
            */
            glDeleteTextures(1, &name);
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName()
        {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const
        {
            // TODO: (Req 5) Complete this function
            /*
             The first parameter specifies the texture target which is here GL_TEXTURE_2D to which the texture object should be bound .
             The second parameter is an unsigned integer value that represents the name of the texture object to be bound
            */
            glBindTexture(GL_TEXTURE_2D, name);
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind()
        {
            // TODO: (Req 5) Complete this function
            /*
            - The first parameter specifies the texture target which is here GL_TEXTURE_2D from which the texture object should be unbound .
            - The second parameter (`0`) represents a special value that unbinds any texture object previously bound to the specified texture target.
            */
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        Texture2D(const Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
    };

}