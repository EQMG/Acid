// kosmos.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "framework.h"

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main()
{
	if (!glfwInit())
	{
		// Initialization failed
		std::cout << "GLFW failed to initalize!" << std::endl;
	}

	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!window)
	{
		// Window or OpenGL context creation failed
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(1.0, 0.0, 0.0, 1.0);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}
