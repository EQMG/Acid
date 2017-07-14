#pragma once

#include <string>
#include <GL/glew.h>

namespace flounder {
	/// <summary>
	/// A struct that represents a shader type.
	/// </summary>
	class shadertype {
	public:
		int m_shaderType;
		std::string m_file;
		std::string m_string;
		GLuint m_shaderID;

		shadertype(int shaderType, std::string file)
		{
			m_shaderType = shaderType;
			m_file = file;
		}

		~shadertype()
		{
		}
	};
}