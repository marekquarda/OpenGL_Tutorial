#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "camera.h"
#include "texture.h"

class Mesh
{
public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    std::vector <Texture> textures;
    // Store VAO in public so it can be used int the Draw function
    VAO VAO;

    // Initializes the mesh
    Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector<Texture>& textures);

    // Draws the mesh
    void Draw(Shader& shader, Camera& Camera);
};

#endif