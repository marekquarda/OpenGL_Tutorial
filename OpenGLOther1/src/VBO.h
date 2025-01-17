#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "glad.h"
#include <glm/glm.hpp>
#include <vector>

// Structure to standardize the vertice used in the meshes
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};


class VBO
{
public:
    // Reference ID of the Vertex Buffer Object 
    GLuint ID;
    // Contructor that generates a Vertex Buffer Object and links it to vertces
    VBO(std::vector<Vertex>& vertices);

    // Binds the VBO
    void Bind();
    // Unbind the VBO
    void Unbind();
    // Deletes the VBO
    void Delete();
};


#endif