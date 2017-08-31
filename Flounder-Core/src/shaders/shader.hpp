#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdarg.h>

#include "../devices/Display.hpp"
#include "../helpers/helperfile.hpp"
#include "../helpers/helperstring.hpp"
#include "../maths/colour.hpp"
#include "../maths/matrix2x2.hpp"
#include "../maths/matrix3x3.hpp"
#include "../maths/matrix4x4.hpp"
#include "../maths/vector2.hpp"
#include "../maths/vector3.hpp"
#include "../maths/vector4.hpp"

#include "shadertype.hpp"

namespace Flounder
{
	class texture;

	/// <summary>
	/// Class that represents a loaded shader.
	/// </summary>
	class shader
	{
	private:
		std::string m_name;
		std::vector<shadertype> *m_types;
		std::vector<VkShaderModule> *m_modules;
		std::vector<VkPipelineShaderStageCreateInfo> *m_stages;
	public:
		/// <summary>
		/// Creates a new shader.
		/// </summary>
		/// <param name="name"> The shaders name. </param>
		/// <param name="n_args"> The number of shader types. </param>
		/// <param name="..."> The list of shader types. </param>
		shader(const std::string &name, const int n_args, ...);

		/// <summary>
		/// Deconstructor for the shader.
		/// </summary>
		~shader();

		/// <summary>
		/// Gets the loaded name for the shader.
		/// </summary>
		/// <returns> The shaders name. </returns>
		inline std::string &getName() { return m_name; }

		std::vector<VkPipelineShaderStageCreateInfo> *getStages() const { return m_stages; }
	private:
		/// <summary>
		/// Creates the shader.
		/// </summary>
		void loadShader();
	};
}
