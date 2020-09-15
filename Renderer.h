#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer
{
	GLFWwindow* window;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int texture;
	int shaderProgram;
	unsigned char* data = nullptr;
	size_t cycle = 0;
public:
	void render();
	void init(GLFWwindow* _window);
	unsigned char* getData() { return data; }
	~Renderer();
};

