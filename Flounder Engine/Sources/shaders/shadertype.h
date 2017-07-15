#pragma once

#include <string>
#include <GL/glew.h>

namespace flounder {
	enum loadtype {
		FILE, STRING
	};

	/// <summary>
	/// A struct that represents a shader type.
	/// </summary>
	class shadertype {
	public:
		GLenum m_shaderType;
		std::string m_load;
		loadtype m_loadtype;
		std::string m_processedString;
		GLuint m_shaderID;

		shadertype(const GLenum &shaderType, const std::string &load, const loadtype &type)
		{
			m_shaderType = shaderType;
			m_load = load;
			m_loadtype = type;
			m_processedString = "";
			m_shaderID = 0;
		}

		~shadertype()
		{
		}
	};
}