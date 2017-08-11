#pragma once

#include <string>

namespace flounder
{
	enum programtype
	{
		VERTEX, 
		TESSELLATION_CONTROL,
		TESSELLATION_EVALUATION,
		GEOMETRY,
		FRAGMENT
	};

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
		programtype m_shaderType;
		std::string m_load;
		loadtype m_loadtype;
		std::string m_processedString;
#if 0
		GLuint m_shaderID;
#endif

		/// <summary>
		/// Creates a new shader type.
		/// </summary>
		/// <param name="shaderType"> The GL shader type. </param>
		/// <param name="load"> The GL string to load. </param>
		/// <param name="type"> The type of GL loading to be done. </param>
		shadertype(const programtype &shaderType, const std::string &load, const loadtype &type)
		{
			m_shaderType = shaderType;
			m_load = load;
			m_loadtype = type;
			m_processedString = "";
#if 0
			m_shaderID = 0;
#endif
		}

		/// <summary>
		/// Deconstructor for the shader type.
		/// </summary>
		~shadertype()
		{
		}
	};
}
