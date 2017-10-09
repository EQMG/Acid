#pragma once

#include <string>
#include <vector>
#include "../platforms/Platform.hpp"
#include "ShaderType.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a loaded shader.
	/// </summary>
	class Shader
	{
	private:
		std::string m_name;
		std::vector<ShaderType> m_types;
		std::vector<VkShaderModule> m_modules;
		std::vector<VkPipelineShaderStageCreateInfo> m_stages;
	public:
		/// <summary>
		/// Creates a new shader.
		/// </summary>
		/// <param name="name"> The shaders name. </param>
		/// <param name="types"> The list of shader types. </param>
		Shader(const std::string &name, const std::vector<ShaderType> &types);

		/// <summary>
		/// Deconstructor for the shader.
		/// </summary>
		~Shader();

		/// <summary>
		/// Gets the loaded name for the shader.
		/// </summary>
		/// <returns> The shaders name. </returns>
		std::string GetName() const { return m_name; }

		std::vector<VkPipelineShaderStageCreateInfo> GetStages() const { return m_stages; }
	};
}
