#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "framework.h"

class display : public module
{
private:
	GLFWwindow* window;
	void error_callback(int error, const char* description);
public:
	display();
	~display();

	void init() override;
	void update() override;
	void render() override;
	void dispose() override;
};

