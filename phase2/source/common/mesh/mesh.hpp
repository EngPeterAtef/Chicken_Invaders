#pragma once

#include "vertex.hpp"
#include <glad/gl.h>

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

class Mesh
{
    // Here, we store the object names of the 3 main components of a mesh:
    // A vertex array object, A vertex buffer and an element buffer
    unsigned int VBO, EBO;
    unsigned int VAO;
    // We need to remember the number of elements that will be draw by glDrawElements
    GLsizei elementCount;

  public:
    glm::vec3 minvertex = {100.0f, 100.0f, 100.0f}; // The minimum coordinates of the bounding box
    glm::vec3 maxvertex = {-1.0f, -1.0f, -1.0f};    // The maximum coordinates of the bounding box
    // The constructor takes two vectors:
    // - vertices which contain the vertex data.
    // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
    // The mesh class does not keep a these data on the RAM. Instead, it should create
    // a vertex buffer to store the vertex data on the VRAM,
    // an element buffer to store the element data on the VRAM,
    // a vertex array object to define how to read the vertex & element buffer during rendering
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
    {
        // TODO: (Req 2) Write this function
        //  remember to store the number of elements in "elementCount" since you will need it for drawing
        //  For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
        elementCount = elements.size();

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

        // glBindBuffer(GL_ARRAY_BUFFER, VBO);

        GLint position_loc = ATTRIB_LOC_POSITION;
        glEnableVertexAttribArray(position_loc);
        glVertexAttribPointer(position_loc, 3, GL_FLOAT, false, sizeof(Vertex), 0);

        GLint color_loc = ATTRIB_LOC_COLOR;
        glEnableVertexAttribArray(color_loc);
        glVertexAttribPointer(color_loc, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void *)offsetof(Vertex, color));

        GLint texture_loc = ATTRIB_LOC_TEXCOORD;
        glEnableVertexAttribArray(texture_loc);
        glVertexAttribPointer(texture_loc, 2, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));

        GLint normal_loc = ATTRIB_LOC_NORMAL;
        glEnableVertexAttribArray(normal_loc);
        glVertexAttribPointer(normal_loc, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindVertexArray(0);

        // finding the min and max vertices to be used in collision detection
        for (auto vertex : vertices)
        {
            if (vertex.position.x < minvertex.x)
                minvertex.x = vertex.position.x;
            if (vertex.position.y < minvertex.y)
                minvertex.y = vertex.position.y;
            if (vertex.position.z < minvertex.z)
                minvertex.z = vertex.position.z;

            if (vertex.position.x > maxvertex.x)
                maxvertex.x = vertex.position.x;
            if (vertex.position.y > maxvertex.y)
                maxvertex.y = vertex.position.y;
            if (vertex.position.z > maxvertex.z)
                maxvertex.z = vertex.position.z;
        }
    }

    // this function should render the mesh
    void draw()
    {
        // TODO: (Req 2) Write this function
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
        // unbind the vertex array object
        glBindVertexArray(0);
    }

    // this function should delete the vertex & element buffers and the vertex array object
    ~Mesh()
    {
        // TODO: (Req 2) Write this function
        // glBindVertexArray(0);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    Mesh(Mesh const &) = delete;
    Mesh &operator=(Mesh const &) = delete;
};

} // namespace our