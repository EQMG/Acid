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
	/// A struct that represents a shader type.
	/// </summary>
	struct shadertype
	{
		programtype m_shaderType;
		std::string m_filePath;
	};
}
