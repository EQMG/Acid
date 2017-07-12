#pragma once

#include <string>
#include <vector>

#include "shadertype.h"

namespace flounder {
	/// <summary>
	/// A builder used to set shader parameters for loading.
	/// </summary>
	class shaderbuilder
	{
	public:
		std::string m_name;
		std::vector<shadertype*> *m_types;

		/// <summary>
		/// Creates a new shader builder.
		/// </summary>
		/// <param name="name"> The name. </param>
		shaderbuilder();

		/// <summary>
		/// Deconstructor for the shader builder.
		/// </summary>
		~shaderbuilder();

		/// <summary>
		/// Sets the name of the shader.
		/// </summary>
		/// <param name="name"> The shaders name. </param>
		/// <returns> This. </returns>
		shaderbuilder *addName(const std::string &name);

		/// <summary>
		/// Adds a new shader type to the load pool.
		/// </summary>
		/// <param name="type"> The shader type to add. </param>
		/// <returns> This. </returns>
		shaderbuilder *addType(shadertype *type);
	};
}
