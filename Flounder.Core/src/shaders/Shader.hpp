#pragma once

#include <string>
#include <vector>

#include "../platforms/glfw/GlfwVulkan.hpp"

#include "ShaderType.hpp"

namespace Flounder
{
	struct AllocatedUniform
	{
		uint32_t bufferOffset;
		VkDescriptorSet descriptorSet;
	};

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
		/// <param name="types"> The list of shader types. </param>
		Shader(const std::string &name, const std::vector<ShaderType> &types);

		/// <summary>
		/// Deconstructor for the shader.
		/// </summary>
		~Shader();

		/// <summary>
		/// Creates the shader.
		/// </summary>
		void Create();

		/// <summary>
		/// Cleans up the shader.
		/// </summary>
		void Cleanup();

		uint8_t *AllocateUniform(const int &size, AllocatedUniform *allocatedUniform);

		/// <summary>
		/// Gets the loaded name for the shader.
		/// </summary>
		/// <returns> The shaders name. </returns>
		std::string GetName() const { return m_name; }

		std::vector<VkPipelineShaderStageCreateInfo> *GetStages() const { return m_stages; }
	};
}
