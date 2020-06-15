#include "PipelineCompute.hpp"

#include "Graphics/Graphics.hpp"
#include "Files/Files.hpp"

namespace acid {
PipelineCompute::PipelineCompute(std::filesystem::path shaderStage, std::vector<Shader::Define> defines, bool pushDescriptors) :
	shaderStage(std::move(shaderStage)),
	defines(std::move(defines)),
	pushDescriptors(pushDescriptors),
	shader(std::make_unique<Shader>()),
	pipelineBindPoint(VK_PIPELINE_BIND_POINT_COMPUTE) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	CreateShaderProgram();
	CreateDescriptorLayout();
	CreateDescriptorPool();
	CreatePipelineLayout();
	CreatePipelineCompute();

#if defined(ACID_DEBUG)
	Log::Out("Pipeline Compute ", this->shaderStage, " created in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

PipelineCompute::~PipelineCompute() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	vkDestroyShaderModule(*logicalDevice, shaderModule, nullptr);

	vkDestroyDescriptorSetLayout(*logicalDevice, descriptorSetLayout, nullptr);
	vkDestroyDescriptorPool(*logicalDevice, descriptorPool, nullptr);
	vkDestroyPipeline(*logicalDevice, pipeline, nullptr);
	vkDestroyPipelineLayout(*logicalDevice, pipelineLayout, nullptr);
}

void PipelineCompute::CmdRender(const CommandBuffer &commandBuffer, const Vector2ui &extent) const {
	auto groupCountX = static_cast<uint32_t>(std::ceil(static_cast<float>(extent.x) / static_cast<float>(*shader->GetLocalSizes()[0])));
	auto groupCountY = static_cast<uint32_t>(std::ceil(static_cast<float>(extent.y) / static_cast<float>(*shader->GetLocalSizes()[1])));
	vkCmdDispatch(commandBuffer, groupCountX, groupCountY, 1);
}

void PipelineCompute::CreateShaderProgram() {
	std::stringstream defineBlock;
	for (const auto &[defineName, defineValue] : defines)
		defineBlock << "#define " << defineName << " " << defineValue << '\n';

	auto fileLoaded = Files::Read(shaderStage);
	if (!fileLoaded)
		throw std::runtime_error("Could not create compute pipeline, missing shader stage");

	auto stageFlag = Shader::GetShaderStage(shaderStage);
	shaderModule = shader->CreateShaderModule(shaderStage, *fileLoaded, defineBlock.str(), stageFlag);

	shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageCreateInfo.stage = stageFlag;
	shaderStageCreateInfo.module = shaderModule;
	shaderStageCreateInfo.pName = "main";

	shader->CreateReflection();
}

void PipelineCompute::CreateDescriptorLayout() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto descriptorSetLayouts = shader->GetDescriptorSetLayouts();

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.flags = pushDescriptors ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR : 0;
	descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayouts.data();
	Graphics::CheckVk(vkCreateDescriptorSetLayout(*logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout));
}

void PipelineCompute::CreateDescriptorPool() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto descriptorPools = shader->GetDescriptorPools();

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolCreateInfo.maxSets = 8192; // 16384;
	descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPools.size());
	descriptorPoolCreateInfo.pPoolSizes = descriptorPools.data();
	Graphics::CheckVk(vkCreateDescriptorPool(*logicalDevice, &descriptorPoolCreateInfo, nullptr, &descriptorPool));
}

void PipelineCompute::CreatePipelineLayout() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto pushConstantRanges = shader->GetPushConstantRanges();

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
	pipelineLayoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();
	Graphics::CheckVk(vkCreatePipelineLayout(*logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));
}

void PipelineCompute::CreatePipelineCompute() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();
	auto pipelineCache = Graphics::Get()->GetPipelineCache();

	VkComputePipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stage = shaderStageCreateInfo;
	pipelineCreateInfo.layout = pipelineLayout;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = -1;
	vkCreateComputePipelines(*logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &pipeline);
}
}
