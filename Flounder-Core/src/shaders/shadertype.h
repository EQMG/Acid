#pragma once

#include <string>

namespace flounder
{
	/// <summary>
	/// A enum that tells the shader how to load the type.
	/// </summary>
	enum loadtype
	{
		FILE,
		STRING
	};

	/// <summary>
	/// A struct that represents a shader type.
	/// </summary>
	struct shadertype
	{
		GLenum m_shaderType;
		std::string m_load;
		loadtype m_loadtype;
		std::string m_processedString;
		GLuint m_shaderID;

		/// <summary>
		/// Creates a new shader type.
		/// </summary>
		/// <param name="shaderType"> The GL shader type. </param>
		/// <param name="load"> The GL string to load. </param>
		/// <param name="type"> The type of GL loading to be done. </param>
		shadertype(const GLenum &shaderType, const std::string &load, const loadtype &type)
		{
			m_shaderType = shaderType;
			m_load = load;
			m_loadtype = type;
			m_processedString = "";
			m_shaderID = 0;
		}

		/// <summary>
		/// Deconstructor for the shader type.
		/// </summary>
		~shadertype()
		{
		}
	};
}
