#include "Engine.h"
#include <iostream>
#include <fstream>
#include <filesystem>
Engine::Engine()
{
	if (!glfwInit())
	{
		// handle error
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 400, "Virtual Console", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		// handle error
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	glfwSwapInterval(1);
	glViewport(0, 0, 640, 400);
}

Engine::~Engine()
{
	glfwTerminate();
}

void Engine::update()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	running = !glfwWindowShouldClose(window);
	if (running) glfwPollEvents();
}

void Engine::init()
{
	std::ifstream f;
	//std::cout << std::filesystem::current_path() << std::endl;
	//f.open("programs/test.txt");
	f.open("programs/screenClear.txt");
	if (!f.is_open())
	{
		std::cout << "file failed to open\n";
		return;
	}
	std::string line;
	std::string code;
	while (std::getline(f, line))
	{
		code += line + "\n";
	}
	//std::cout << code << std::endl;
	f.close();
	//program = assembler.compile("MARK #TOP LIT 255 POP $ACC MARK #LOOP JLZ #DONE DEC PUSH $ACC CLEAR VBL JMP #LOOP MARK #DONE JMP #TOP");
	program = assembler.compile(code);
	renderer.init(window);
	audio.init();
	interpreter.loadProgram(program);
	//interpreter.run();
	//interpreter.run();
	//interpreter.setVbuffer(renderer.getData());

	while (running)
	{
		interpreter.run();
		interpreter.setVbuffer(renderer.getData());
		renderer.render();
		audio.update();
		update();
	}
	free(program);
}