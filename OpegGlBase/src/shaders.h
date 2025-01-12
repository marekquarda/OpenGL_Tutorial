#ifndef SHADERS_CLASS_H
#define SHADERS_CLASS_H

#include <string>

std::string vsSrc = 
R".(
   #version 450

    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aColor;

    out vec3 color;

    void main() {
	    gl_Position = vec4(aPos.x, aPos.y, aPos.y, 1.0);
	    color = aColor;
    }
).";

std::string fsSrc = 
R".(
    #version 450

    out vec4 FragColor;
    in vec3 color;

    void main() {
	    FragColor = vec4(color,1.0f);
    }
).";

#endif