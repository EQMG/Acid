#pragma once

#include <GL\glew.h>
#include <vector>
#include <map>

#include "../framework/framework.h"

namespace flounder {
	/// <summary>
	/// A module used for loading and managing OpenGL VAO's and VBO's.
	/// </summary>
	class loaders : public imodule
	{
	private:
		std::map<GLuint*, std::vector<int>*> *m_loaded;
	public:
		static loaders *get()
		{
			return (loaders*)framework::get()->getInstance("loaders");
		}

		loaders();

		~loaders();

		void init();

		void update();
	};
}