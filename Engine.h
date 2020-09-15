#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdint.h>
#include "Renderer.h"
#include "Audio.h"
#include "Interpreter.h"
#include "Assembler.h"

class Engine
{
	GLFWwindow* window;
	bool running = true;
	Renderer renderer;
	Audio audio;
	Interpreter interpreter;
	Assembler assembler;
	uint8_t* program;
public:
	Engine();
	~Engine();
	void update();
	void init();
};

