#include "Shader.hpp"

namespace Flounder
{
	Shader::Shader(const std::string &name, const int n_args, ...) :
		m_name(name),
		m_types(new std::vector<ShaderType>()),
		m_modules(new std::vector<VkShaderModule>()),
		m_stages(new std::vector<VkPipelineShaderStageCreateInfo>())
	{
		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_types->push_back(va_arg(ap, ShaderType));
		}

		va_end(ap);

		LoadShader();
	}

	Shader::~Shader()
	{
		for (auto shaderModule : *m_modules)
		{
			vkDestroyShaderModule(Display::Get()->GetVkDevice(), shaderModule, nullptr);
		}

		delete m_types;
		delete m_stages;
	}

	void Shader::LoadShader() const
	{
		for (ShaderType type : *m_types)
		{
			std::vector<char> shaderCode = HelperFile::ReadBinaryFile(type.GetFilePath());

			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = shaderCode.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

			VkShaderModule shaderModule = VK_NULL_HANDLE;
			GlfwVulkan::ErrorCheck(vkCreateShaderModule(Display::Get()->GetVkDevice(), &createInfo, nullptr, &shaderModule));

			VkPipelineShaderStageCreateInfo shaderStageInfo = {};
			shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageInfo.stage = type.GetShaderFlag();
			shaderStageInfo.module = shaderModule;
			shaderStageInfo.pName = "main"; // The shaders entry point.

			m_modules->push_back(shaderModule);
			m_stages->push_back(shaderStageInfo);
		}
	}
}
