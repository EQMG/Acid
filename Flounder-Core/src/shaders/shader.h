#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdarg.h>

#include "../devices/display.h"
#include "../helpers/helperfile.h"
#include "../helpers/helperstring.h"
#include "../maths/colour.h"
#include "../maths/matrix2x2.h"
#include "../maths/matrix3x3.h"
#include "../maths/matrix4x4.h"
#include "../maths/vector2.h"
#include "../maths/vector3.h"
#include "../maths/vector4.h"

#include "shadertype.h"

namespace flounder
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
		void create();

		std::vector<char> readFile(const std::string& fileName);
	};
}
