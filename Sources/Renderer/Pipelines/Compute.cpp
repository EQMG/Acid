#include "Compute.hpp"

#include <cmath>
#include <cassert>
#include "Display/Display.hpp"
#include "Helpers/FileSystem.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	Compute::Compute(const std::string &shaderStage, const uint32_t &width, const uint32_t &height,
	    const uint32_t &workgroupSize, const std::vector<ShaderDefine> &defines) :
		IPipeline(),
		m_shaderStage(shaderStage),
		m_width(width),
		m_height(height),
		m_workgroupSize(workgroupSize),
		m_defines(defines),
		m_shaderProgram(std::make_unique<ShaderProgram>(m_shaderStage)),
		m_shaderModule(VK_NULL_HANDLE),
		m_shaderStageCreateInfo({}),
		m_descriptorSetLayout(VK_NULL_HANDLE),
		m_descriptorPool(VK_NULL_HANDLE),
		m_pipeline(VK_NULL_HANDLE),
		m_pipelineLayout(VK_NULL_HANDLE)
	{
#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		m_defines.emplace_back("WIDTH", String::To(m_width));
		m_defines.emplace_back("HEIGHT", String::To(m_height));
		m_defines.emplace_back("WORKGROUP_SIZE", String::To(m_workgroupSize));

		CreateShaderProgram();
		CreateDescriptorLayout();
		CreateDescriptorPool();
		CreatePipelineLayout();
		CreatePipelineCompute();

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
	//	Log::Out("%s", m_shaderProgram->ToString().c_str());
		Log::Out("Compute pipeline '%s' created in %ims\n", m_shaderStage.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif
	}

	Compute::~Compute()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		Display::CheckVk(vkDeviceWaitIdle(logicalDevice));

		vkDestroyShaderModule(logicalDevice, m_shaderModule, nullptr);

		vkDestroyDescriptorSetLayout(logicalDevice, m_descriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, nullptr);
		vkDestroyPipeline(logicalDevice, m_pipeline, nullptr);
		vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
	}

	bool Compute::CmdRender(const CommandBuffer &commandBuffer) const
	{
		auto groupCountX = static_cast<uint32_t>(std::ceil(static_cast<float>(m_width) / static_cast<float>(m_workgroupSize)));
		auto groupCountY = static_cast<uint32_t>(std::ceil(static_cast<float>(m_height) / static_cast<float>(m_workgroupSize)));
		vkCmdDispatch(commandBuffer.GetCommandBuffer(), groupCountX, groupCountY, 1);
		return true;
	}

	void Compute::CreateShaderProgram()
	{
		std::stringstream defineBlock;
		defineBlock << "\n";

		for (auto &define : m_defines)
		{
			defineBlock << "#define " << define.first << " " << define.second << "\n";
		}

		auto fileLoaded = Files::Read(m_shaderStage);

		if (!fileLoaded)
		{
			Log::Error("Shader Stage could not be loaded: '%s'\n", m_shaderStage.c_str());
			assert(false && "Could not create compute pipeline, missing shader stage!");
			return;
		}

		auto shaderCode = ShaderProgram::InsertDefineBlock(*fileLoaded, defineBlock.str());
		shaderCode = ShaderProgram::ProcessIncludes(shaderCode);

		auto stageFlag = ShaderProgram::GetShaderStage(m_shaderStage);
		m_shaderModule = m_shaderProgram->ProcessShader(shaderCode, stageFlag);

		m_shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStageCreateInfo.stage = stageFlag;
		m_shaderStageCreateInfo.module = m_shaderModule;
		m_shaderStageCreateInfo.pName = "main";

		m_shaderProgram->ProcessShader();
	}

	void Compute::CreateDescriptorLayout()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		auto descriptorSetLayouts = m_shaderProgram->GetDescriptorSetLayouts();

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayouts.data();
		Display::CheckVk(vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
	}

	void Compute::CreateDescriptorPool()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		auto descriptorPools = m_shaderProgram->GetDescriptorPools();

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolCreateInfo.maxSets = 16384;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPools.size());
		descriptorPoolCreateInfo.pPoolSizes = descriptorPools.data();
		Display::CheckVk(vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));
	}

	void Compute::CreatePipelineLayout()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;
		Display::CheckVk(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
	}

	void Compute::CreatePipelineCompute()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto pipelineCache = Renderer::Get()->GetPipelineCache();

		VkComputePipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.stage = m_shaderStageCreateInfo;
		pipelineCreateInfo.layout = m_pipelineLayout;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;
		vkCreateComputePipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline);
	}
}
