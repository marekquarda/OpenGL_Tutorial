#include "shaderClass.h"

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


// Reads a text file and outputs a string everything in the text file
std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if(in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0],contents.size());
        in.close();
        return (contents);
    }
    throw(errno);
}

// Contructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile) 
{
    // Read vertextFile and fragmentFile and store the string
    std::string vertexCode = vsSrc;// get_file_contents(vertexFile);
    std::string fragmentCode = fsSrc; // get_file_contents(fragmentFile);

    // Convert the shader source strings into character arrays
    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // Create Vertex Shader Object and get its reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Compile the Vertex Shader into machine code
    glCompileShader(vertexShader);

    // Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

    // Create Shader Program Object and get its reference
    ID = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(ID);

    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Activate the Shader Program
void Shader::Activate()
{
    glUseProgram(ID);
}

// Delete the Shader Program
void Shader::Delete()
{
    glDeleteProgram(ID);
}
