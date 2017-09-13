#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdarg.h>

#include "../helpers/HelperFile.hpp"
#include "../platforms/glfw/GlfwVulkan.hpp"

#include "ShaderType.hpp"

namespace Flounder
{
	class Texture;

	/// <summary>
	/// Class that represents a loaded shader.
	/// </summary>
	class Shader
	{
	private:
		std::string m_name;
		std::vector<ShaderType> *m_types;
		std::vector<VkShaderModule> *m_modules;
		std::vector<VkPipelineShaderStageCreateInfo> *m_stages;
	public:
		/// <summary>
		/// Creates a new shader.
		/// </summary>
		/// <param name="name"> The shaders name. </param>
		/// <param name="n_args"> The number of shader types. </param>
		/// <param name="..."> The list of shader types. </param>
		Shader(const std::string &name, const int n_args, ...);

		/// <summary>
		/// Deconstructor for the shader.
		/// </summary>
		~Shader();

		/// <summary>
		/// Creates the shader.
		/// </summary>
		void Create(const VkDevice *logicalDevice);

		/// <summary>
		/// Cleans up the shader.
		/// </summary>
		void Cleanup(const VkDevice *logicalDevice);

		/// <summary>
		/// Gets the loaded name for the shader.
		/// </summary>
		/// <returns> The shaders name. </returns>
		std::string GetName() const { return m_name; }

		std::vector<VkPipelineShaderStageCreateInfo> *GetStages() const { return m_stages; }
	};
}
