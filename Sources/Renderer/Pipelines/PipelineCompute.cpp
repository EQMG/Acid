#include "PipelineCompute.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>
#include "Renderer/Renderer.hpp"
#include "Files/FileSystem.hpp"

namespace acid
{
	PipelineCompute::PipelineCompute(std::string shaderStage, const uint32_t &width, const uint32_t &height, const uint32_t &workgroupSize, 
		const bool &pushDescriptors, std::vector<Shader::Define> defines) :
		m_shaderStage(std::move(shaderStage)),
		m_width(width),
		m_height(height),
		m_workgroupSize(workgroupSize),
		m_pushDescriptors(pushDescriptors),
		m_defines(std::move(defines)),
		m_shader(std::make_unique<Shader>(m_shaderStage)),
		m_shaderModule(VK_NULL_HANDLE),
		m_shaderStageCreateInfo({}),
		m_descriptorSetLayout(VK_NULL_HANDLE),
		m_descriptorPool(VK_NULL_HANDLE),
		m_pipeline(VK_NULL_HANDLE),
		m_pipelineLayout(VK_NULL_HANDLE),
		m_pipelineBindPoint(VK_PIPELINE_BIND_POINT_COMPUTE)
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
	//	Log::Out("%s", m_shader->ToString().c_str());
		Log::Out("PipelineCompute pipeline '%s' created in %ims\n", m_shaderStage.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif
	}

	PipelineCompute::~PipelineCompute()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		vkDestroyShaderModule(logicalDevice->GetLogicalDevice(), m_shaderModule, nullptr);

		vkDestroyDescriptorSetLayout(logicalDevice->GetLogicalDevice(), m_descriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(logicalDevice->GetLogicalDevice(), m_descriptorPool, nullptr);
		vkDestroyPipeline(logicalDevice->GetLogicalDevice(), m_pipeline, nullptr);
		vkDestroyPipelineLayout(logicalDevice->GetLogicalDevice(), m_pipelineLayout, nullptr);
	}

	bool PipelineCompute::CmdRender(const CommandBuffer &commandBuffer) const
	{
		auto groupCountX = static_cast<uint32_t>(std::ceil(static_cast<float>(m_width) / static_cast<float>(m_workgroupSize)));
		auto groupCountY = static_cast<uint32_t>(std::ceil(static_cast<float>(m_height) / static_cast<float>(m_workgroupSize)));
		vkCmdDispatch(commandBuffer.GetCommandBuffer(), groupCountX, groupCountY, 1);
		return true;
	}

	void PipelineCompute::CreateShaderProgram()
	{
		std::stringstream defineBlock;
		defineBlock << "\n";

		for (const auto &define : m_defines)
		{
			defineBlock << "#define " << define.first << " " << define.second << "\n";
		}

		auto fileLoaded = Files::Read(m_shaderStage);

		if (!fileLoaded)
		{
			Log::Error("Shader Stage could not be loaded: '%s'\n", m_shaderStage.c_str());
			throw std::runtime_error("Could not create compute pipeline, missing shader stage");
			return;
		}

		auto shaderCode = Shader::InsertDefineBlock(*fileLoaded, defineBlock.str());
		shaderCode = Shader::ProcessIncludes(shaderCode);

		auto stageFlag = Shader::GetShaderStage(m_shaderStage);
		m_shaderModule = m_shader->ProcessShader(shaderCode, stageFlag);

		m_shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStageCreateInfo.stage = stageFlag;
		m_shaderStageCreateInfo.module = m_shaderModule;
		m_shaderStageCreateInfo.pName = "main";

		m_shader->ProcessShader();
	}

	void PipelineCompute::CreateDescriptorLayout()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		auto descriptorSetLayouts = m_shader->GetDescriptorSetLayouts();

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.flags = m_pushDescriptors ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR : 0;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayouts.data();
		Renderer::CheckVk(vkCreateDescriptorSetLayout(logicalDevice->GetLogicalDevice(), &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
	}

	void PipelineCompute::CreateDescriptorPool()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		auto descriptorPools = m_shader->GetDescriptorPools();

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolCreateInfo.maxSets = 16384;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPools.size());
		descriptorPoolCreateInfo.pPoolSizes = descriptorPools.data();
		Renderer::CheckVk(vkCreateDescriptorPool(logicalDevice->GetLogicalDevice(), &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));
	}

	void PipelineCompute::CreatePipelineLayout()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;
		Renderer::CheckVk(vkCreatePipelineLayout(logicalDevice->GetLogicalDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
	}

	void PipelineCompute::CreatePipelineCompute()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		auto pipelineCache = Renderer::Get()->GetPipelineCache();

		VkComputePipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.stage = m_shaderStageCreateInfo;
		pipelineCreateInfo.layout = m_pipelineLayout;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;
		vkCreateComputePipelines(logicalDevice->GetLogicalDevice(), pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline);
	}
}
