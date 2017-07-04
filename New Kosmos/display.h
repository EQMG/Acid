#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "framework.h"

namespace flounder {
	class display : public module
	{
	private:
		GLFWwindow* window;
	public:
		display();
		~display();

		void init() override;
		void update() override;
		void render() override;
		void dispose() override;
	};
}
