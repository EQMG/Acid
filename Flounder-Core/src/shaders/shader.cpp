#include "shader.h"

namespace flounder
{
	shader::shader(const std::string &name, const int n_args, ...)
	{
		m_name = name;
		m_types = new std::vector<shadertype>();
		m_modules = new std::vector<VkShaderModule>();
		m_stages = new std::vector<VkPipelineShaderStageCreateInfo>();

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_types->push_back(va_arg(ap, shadertype));
		}

		va_end(ap);

		create();
	}

	shader::~shader()
	{
		for (VkShaderModule shaderModule : *m_modules)
		{
			vkDestroyShaderModule(display::get()->getVkDevice(), shaderModule, nullptr);
		}

		delete m_types;
		delete m_stages;
	}

	void shader::create()
	{
		for (shadertype type : *m_types)
		{
			std::vector<char> shaderCode = readFile(type.m_filePath);

			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = shaderCode.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

			VkShaderModule shaderModule = VK_NULL_HANDLE;
			display::vkErrorCheck(vkCreateShaderModule(display::get()->getVkDevice(), &createInfo, nullptr, &shaderModule));

			VkPipelineShaderStageCreateInfo shaderStageInfo = {};
			shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			
			switch(type.m_shaderType)
			{
			case VERTEX:
				shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case TESSELLATION_CONTROL:
				shaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				break;
			case TESSELLATION_EVALUATION:
				shaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
				break;
			case GEOMETRY:
				shaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				break;
			case FRAGMENT:
				shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			case COMPUTE:
				shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
				break;
			}

			shaderStageInfo.module = shaderModule;
			shaderStageInfo.pName = m_name.c_str();

			m_modules->push_back(shaderModule);
			m_stages->push_back(shaderStageInfo);
		}
	}

	std::vector<char> shader::readFile(const std::string &fileName)
	{
		std::ifstream file(fileName, std::ios::ate | std::ios::binary);

		if (!file.is_open()) 
		{
			throw std::runtime_error("Failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);		file.close();

		return buffer;
	}
}
