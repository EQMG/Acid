#include "PipelineCompute.hpp"

#include "Graphics/Graphics.hpp"
#include "Files/FileSystem.hpp"

namespace acid
{
PipelineCompute::PipelineCompute(std::string shaderStage, std::vector<Shader::Define> defines, const bool &pushDescriptors) :
	m_shaderStage(std::move(shaderStage)),
	m_defines(std::move(defines)),
	m_pushDescriptors(pushDescriptors),
	m_shader(std::make_unique<Shader>()),
	m_shaderModule(VK_NULL_HANDLE),
	m_shaderStageCreateInfo({}),
	m_descriptorSetLayout(VK_NULL_HANDLE),
	m_descriptorPool(VK_NULL_HANDLE),
	m_pipeline(VK_NULL_HANDLE),
	m_pipelineLayout(VK_NULL_HANDLE),
	m_pipelineBindPoint(VK_PIPELINE_BIND_POINT_COMPUTE)
{
#if defined(ACID_VERBOSE)
	auto debugStart = Time::Now();
#endif

	CreateShaderProgram();
	CreateDescriptorLayout();
	CreateDescriptorPool();
	CreatePipelineLayout();
	CreatePipelineCompute();

#if defined(ACID_VERBOSE)
	auto debugEnd = Time::Now();
	//Log::Out("%s", m_shader->ToString().c_str());
	Log::Out("Pipeline compute '%s' created in %.3fms\n", m_shaderStage.c_str(), (debugEnd - debugStart).AsMilliseconds<float>());
#endif
}

PipelineCompute::~PipelineCompute()
{
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	vkDestroyShaderModule(*logicalDevice, m_shaderModule, nullptr);

	vkDestroyDescriptorSetLayout(*logicalDevice, m_descriptorSetLayout, nullptr);
	vkDestroyDescriptorPool(*logicalDevice, m_descriptorPool, nullptr);
	vkDestroyPipeline(*logicalDevice, m_pipeline, nullptr);
	vkDestroyPipelineLayout(*logicalDevice, m_pipelineLayout, nullptr);
}

void PipelineCompute::CmdRender(const CommandBuffer &commandBuffer, const Vector2ui &extent) const
{
	auto groupCountX = static_cast<uint32_t>(std::ceil(static_cast<float>(extent.m_x) / static_cast<float>(*m_shader->GetLocalSizes()[0])));
	auto groupCountY = static_cast<uint32_t>(std::ceil(static_cast<float>(extent.m_y) / static_cast<float>(*m_shader->GetLocalSizes()[1])));
	vkCmdDispatch(commandBuffer, groupCountX, groupCountY, 1);
}

void PipelineCompute::CreateShaderProgram()
{
	std::stringstream defineBlock;

	for (const auto &define : m_defines)
	{
		defineBlock << "#define " << define.first << " " << define.second << "\n";
	}

	auto fileLoaded = Files::Read(m_shaderStage);

	if (!fileLoaded)
	{
		Log::Error("Shader Stage could not be loaded: '%s'\n", m_shaderStage.c_str());
		throw std::runtime_error("Could not create compute pipeline, missing shader stage");
	}

	auto stageFlag = Shader::GetShaderStage(m_shaderStage);
	m_shaderModule = m_shader->CreateShaderModule(m_shaderStage, *fileLoaded, defineBlock.str(), stageFlag);

	m_shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	m_shaderStageCreateInfo.stage = stageFlag;
	m_shaderStageCreateInfo.module = m_shaderModule;
	m_shaderStageCreateInfo.pName = "main";

	m_shader->CreateReflection();
}

void PipelineCompute::CreateDescriptorLayout()
{
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto descriptorSetLayouts = m_shader->GetDescriptorSetLayouts();

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.flags = m_pushDescriptors ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR : 0;
	descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayouts.data();
	Graphics::CheckVk(vkCreateDescriptorSetLayout(*logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
}

void PipelineCompute::CreateDescriptorPool()
{
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto descriptorPools = m_shader->GetDescriptorPools();

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolCreateInfo.maxSets = 8192; // 16384;
	descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPools.size());
	descriptorPoolCreateInfo.pPoolSizes = descriptorPools.data();
	Graphics::CheckVk(vkCreateDescriptorPool(*logicalDevice, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));
}

void PipelineCompute::CreatePipelineLayout()
{
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto pushConstantRanges = m_shader->GetPushConstantRanges();

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
	pipelineLayoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();
	Graphics::CheckVk(vkCreatePipelineLayout(*logicalDevice, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
}

void PipelineCompute::CreatePipelineCompute()
{
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();
	auto pipelineCache = Graphics::Get()->GetPipelineCache();

	VkComputePipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stage = m_shaderStageCreateInfo;
	pipelineCreateInfo.layout = m_pipelineLayout;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = -1;
	vkCreateComputePipelines(*logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline);
}
}
