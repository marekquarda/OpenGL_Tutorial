#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "glad.h"

class VBO
{
public:
    // Reference ID of the Vertex Buffer Object 
    GLuint ID;
    // Contructor that generates a Vertex Buffer Object and links it to vertces
    VBO(GLfloat* vertices,GLsizeiptr size);

    // Binds the VBO
    void Bind();
    // Unbind the VBO
    void Unbind();
    // Deletes the VBO
    void Delete();
};


#endif