#include "Shader.hpp"

#include "../devices/Display.hpp"
#include "../helpers/HelperFile.hpp"

namespace Flounder
{
	Shader::Shader(const std::string &name, const std::vector<ShaderType> &types) :
		m_name(name),
		m_types(std::vector<ShaderType>(types)),
		m_modules(std::vector<VkShaderModule>()),
		m_stages(std::vector<VkPipelineShaderStageCreateInfo>())
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		for (auto type : m_types)
		{
			std::vector<char> shaderCode = HelperFile::ReadBinaryFile(type.GetFilePath());

			VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
			shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shaderModuleCreateInfo.codeSize = shaderCode.size();
			shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

			VkShaderModule shaderModule = VK_NULL_HANDLE;
			Platform::ErrorVk(vkCreateShaderModule(logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule));

			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
			pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pipelineShaderStageCreateInfo.stage = type.GetShaderFlag();
			pipelineShaderStageCreateInfo.module = shaderModule;
			pipelineShaderStageCreateInfo.pName = "main"; // The shaders entry point.

			m_modules.push_back(shaderModule);
			m_stages.push_back(pipelineShaderStageCreateInfo);
		}

#if FLOUNDER_VERBOSE
		printf("Loading Shader: '%s'\n", name.c_str());
#endif
	}

	Shader::~Shader()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		for (auto shaderModule : m_modules)
		{
			vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
		}
	}
}
