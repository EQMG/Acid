#include "display.h"

namespace flounder {
	display::display()
		: module(module::ALWAYS, std::vector<module*>())
	{
	}

	display::~display()
	{
	}

	void display::init()
	{
		if (!glfwInit())
		{
			// Initialization failed
			std::cout << "GLFW failed to initalize!" << std::endl;
		}

		// glfwSetErrorCallback(error_callback);

		window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

		if (!window)
		{
			// Window or OpenGL context creation failed
		}

		glfwMakeContextCurrent(window);
	}

	void display::update()
	{
		glfwPollEvents();
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(1.0, 0.0, 0.0, 1.0);
	}

	void display::render()
	{
		glfwSwapBuffers(window);
	}

	void display::dispose()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}