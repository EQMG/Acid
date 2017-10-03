#include "Shader.hpp"

#include <map>
#include "../devices/Display.hpp"
#include "../helpers/HelperFile.hpp"

namespace Flounder
{
	Shader::Shader(const std::string &name, const std::vector<ShaderType> &types) :
		m_name(name),
		m_types(new std::vector<ShaderType>(types)),
		m_modules(new std::vector<VkShaderModule>()),
		m_stages(new std::vector<VkPipelineShaderStageCreateInfo>())
	{
	}

	Shader::~Shader()
	{
	}

	void Shader::Create()
	{
		const auto logicalDevice = Display::Get()->GetDevice();

		for (auto type : *m_types)
		{
			std::vector<char> shaderCode = HelperFile::ReadBinaryFile(type.GetFilePath());

			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = shaderCode.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

			VkShaderModule shaderModule = VK_NULL_HANDLE;
			GlfwVulkan::ErrorVk(vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule));

			VkPipelineShaderStageCreateInfo shaderStageInfo = {};
			shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageInfo.stage = type.GetShaderFlag();
			shaderStageInfo.module = shaderModule;
			shaderStageInfo.pName = "main"; // The shaders entry point.

			m_modules->push_back(shaderModule);
			m_stages->push_back(shaderStageInfo);
		}
	}

	void Shader::Cleanup()
	{
		const auto logicalDevice = Display::Get()->GetDevice();

		for (auto shaderModule : *m_modules)
		{
			vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
		}

		//	delete m_types;
		//	delete m_stages;
	}

	uint8_t *Shader::AllocateUniform(const int &size, AllocatedUniform *allocatedUniform)
	{
		const int align_mod = size % 256;
		const int aligned_size = ((size % 256) == 0) ? size : (size + 256 - align_mod);
		//	allocatedUniform->bufferOffset = current_offset;
		//	allocatedUniform->descriptorSet = descriptor_set;
		return 0;
	}
}
