#ifndef SHADERS_CLASS_H
#define SHADERS_CLASS_H

#include <string>

std::string vsSrc = 
R".(
   #version 450
    // Positions/Coordinates
    layout (location = 0) in vec3 aPos;
    // Colors
    layout (location = 1) in vec3 aColor;
    // Texture Coordinates
    layout (location = 2) in vec2 aTex;

    // Outputs the color for the Fragment Shader
    out vec3 color;
    // Outputs the texture coordinates to the fragment shader
    out vec2 texCoord;

    // Controls the scale of the vertices
    uniform float scale;

    // Imports the camera matrix from the main function
    uniform mat4 camMatrix;

    void main()
    {
	    // Outputs the positions/coordinates of all vertices
	    gl_Position = camMatrix * vec4(aPos, 1.0);
	    // Assigns the colors from the Vertex Data to "color"
	    color = aColor;
	    // Assigns the texture coordinates from the Vertex Data to "texCoord"
	    texCoord = aTex;
    }
).";

std::string fsSrc = 
R".(
    #version 450
    // Outputs colors in RGBA
    out vec4 FragColor;

    // Inputs the color from the Vertex Shader
    in vec3 color;
    // Inputs the texture coordinates from the Vertex Shader
    in vec2 texCoord;

    // Gets the Texture Unit from the main function
    uniform sampler2D tex0;

    void main()
    {
	    FragColor = texture(tex0, texCoord);
    }
).";

#endif