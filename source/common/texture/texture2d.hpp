#pragma once

#include <glad/gl.h>

namespace our {

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D {
        // The OpenGL object name of this texture 
        GLuint name = 0;
    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name" 
        Texture2D() {
            //TODO: (Req 5) Complete this function
            //generate one texture related to the name object
            glGenTextures(1, &name);
            //bind the texture to the GL_TEXTURE_2D
            glBindTexture(GL_TEXTURE_2D, name);
            //set the texture parameters
            // GL_TEXTURE_WRAP_S: set the texture wrap parameter for texture coordinate s(X-axis)
            // GL_TEXTURE_WRAP_T: set the texture wrap parameter for texture coordinate t(Y-axis)
            // GL_REPEAT: repeat the texture image
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // GL_TEXTURE_MIN_FILTER: set texture minifying function
            // GL_TEXTURE_MAG_FILTER: set texture magnification function
            // GL_LINEAR: linear interpolation
            //  GL_NEAREST_MIPMAP_LINEAR: choose the mipmap that most closely matches the size of the pixel being textured and use the GL_LINEAR criterion (the texture element closest to the center of the pixel) to produce a texture value
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D() { 
            //TODO: (Req 5) Complete this function
            glDeleteTextures(1, &name);
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName() {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const {
            //TODO: (Req 5) Complete this function
            glBindTexture(GL_TEXTURE_2D, name);
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind(){
            //TODO: (Req 5) Complete this function
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
    };
    
}