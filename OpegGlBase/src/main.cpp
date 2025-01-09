#include <iostream>
#include "glad.h"
#include <glfw/glfw3.h>


const char* vSrc = 
R".(
    #version 460

	layout(location = 0) in vec3 aPos;

    void main() {
		gl_Position = vec4(aPos.x, aPos.y, aPos.y, 1.0);
    }
).";

const char* fSrc = 		
R".(
    #version 460

	out vec4 FragColor;

    void main() {
		FragColor = vec4(0.8f,0.3f,0.02f,1.0f);
    }
).";


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] = 
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
	};	

	GLuint indices[] = 
	{
		0 , 3 , 5,
		3 , 2 , 4,
		5 , 4 , 1
	};

	GLFWwindow *window = glfwCreateWindow(800,800,"OpenGl Base",NULL, NULL);
	if(window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0,0,800,800);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vSrc, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fSrc, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// remove shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint VBO,VAO,EBO;
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
 
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glClearColor(1.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(window);

	while(!glfwWindowShouldClose(window)) {
		glClearColor(0.07f,0.13f,0.17f,1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0,3);
		glDrawElements(GL_TRIANGLES,9, GL_UNSIGNED_INT,0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1,&EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}