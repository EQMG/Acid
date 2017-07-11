#pragma once

#include <string>
#include <GL/glew.h>
#include "../shader.h"

namespace flounder {
	/// <summary>
	/// Represents a uniform variable uploaded from Java to OpenGL shaders.
	/// </summary>
	class iuniform 
	{
	private:
		static const int NOT_FOUND = -1;

		std::string m_name;
		shader *m_programID;

		int m_location;
	protected:
		iuniform(const std::string &name, shader *shader)
		{
			m_name = name;
			m_programID = shader;
		}

		inline int getLocation() { return m_location; }
	public:
		void storeUniformLocation(int program)
		{
			m_location = glGetUniformLocation(program, m_name.c_str());

			if (m_location == NOT_FOUND)
			{
			//	logger::get()->warning("No uniform variable called " + name + " found in shader " + shader->getName() + "!");
			}
		}
	};
}