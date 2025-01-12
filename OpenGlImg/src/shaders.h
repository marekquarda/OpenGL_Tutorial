#ifndef SHADERS_CLASS_H
#define SHADERS_CLASS_H

#include <string>

std::string vsSrc = 
R".(
   #version 450

    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aColor;
    layout(location = 2) in vec2 aTex;  

    out vec3 color;
    out vec2 texCoord;

    void main() {
	    gl_Position = vec4(aPos.x, aPos.y, aPos.y, 1.0);
	    color = aColor;
        texCoord = aTex;
    }
).";

std::string fsSrc = 
R".(
    #version 450

    out vec4 FragColor;
    in vec3 color;
    in vec2 texCoord;
    uniform sampler2D tex0;

    void main() {
	    //FragColor = vec4(color,1.0f);
        FragColor = texture(tex0, texCoord);
    }
).";

#endif