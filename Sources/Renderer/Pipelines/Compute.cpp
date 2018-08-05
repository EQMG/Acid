#include "Compute.hpp"

#include <cmath>
#include "Helpers/FileSystem.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	Compute::Compute(const std::string &shaderStage) :
		IPipeline(),
		m_shaderStage(Files::SearchFile(shaderStage)),
		m_shaderProgram(std::make_shared<ShaderProgram>(shaderStage)),
		m_shaderModule(VK_NULL_HANDLE),
		m_shaderStageCreateInfo({}),
		m_descriptorSetLayout(VK_NULL_HANDLE),
		m_descriptorPool(VK_NULL_HANDLE),
		m_pipeline(VK_NULL_HANDLE),
		m_pipelineLayout(VK_NULL_HANDLE)
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		CreateShaderProgram();
		CreateDescriptorLayout();
		CreateDescriptorPool();
		CreatePipelineLayout();
		CreatePipelineCompute();

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
	//	fprintf(stdout, "%s", m_shaderProgram->ToString().c_str());
		fprintf(stdout, "Compute pipeline '%s' created in %fms\n", m_shaderStage.c_str(), debugEnd - debugStart);
#endif
	}

	Compute::~Compute()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		vkDeviceWaitIdle(logicalDevice);

		vkDestroyShaderModule(logicalDevice, m_shaderModule, allocator);

		vkDestroyDescriptorSetLayout(logicalDevice, m_descriptorSetLayout, allocator);
		vkDestroyDescriptorPool(logicalDevice, m_descriptorPool, allocator);
		vkDestroyPipeline(logicalDevice, m_pipeline, allocator);
		vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, allocator);
	}

	void Compute::CmdRender(const CommandBuffer &commandBuffer, const uint32_t &groupCountX, const uint32_t &groupCountY, const uint32_t &groupCountZ) const
	{
		vkCmdDispatch(commandBuffer.GetVkCommandBuffer(), groupCountX, groupCountY, groupCountZ);
	}

	void Compute::CreateShaderProgram()
	{
		std::stringstream defineBlock;
		defineBlock << "\n";

	//	for (auto &define : m_defines)
	//	{
	//		defineBlock << "#define " << define.GetName() << " " << define.GetValue() << "\n";
	//	}

		if (!FileSystem::FileExists(m_shaderStage))
		{
			fprintf(stderr, "File does not exist: '%s'\n", m_shaderStage.c_str());
			throw std::runtime_error("Could not create pipeline, missing shader stage!");
		}

		auto fileLoaded = FileSystem::ReadTextFile(m_shaderStage);

		if (!fileLoaded.has_value())
		{
			return;
		}

		auto shaderCode = ShaderProgram::InsertDefineBlock(fileLoaded.value(), defineBlock.str());
		shaderCode = ShaderProgram::ProcessIncludes(shaderCode);

		VkShaderStageFlagBits stageFlag = ShaderProgram::GetShaderStage(m_shaderStage);
		m_shaderModule = m_shaderProgram->ProcessShader(shaderCode, stageFlag);

		m_shaderStageCreateInfo = {};
		m_shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStageCreateInfo.stage = stageFlag;
		m_shaderStageCreateInfo.module = m_shaderModule;
		m_shaderStageCreateInfo.pName = "main";

		m_shaderProgram->ProcessShader();
	}

	void Compute::CreateDescriptorLayout()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		std::vector<VkDescriptorSetLayoutBinding> bindings = std::vector<VkDescriptorSetLayoutBinding>();

		for (auto &type : m_shaderProgram->GetDescriptors())
		{
			bindings.emplace_back(type.GetLayoutBinding());
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		descriptorSetLayoutCreateInfo.pBindings = bindings.data();

		vkDeviceWaitIdle(logicalDevice);
		Display::CheckVk(vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, allocator, &m_descriptorSetLayout));
	}

	void Compute::CreateDescriptorPool()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		std::vector<VkDescriptorPoolSize> poolSizes = std::vector<VkDescriptorPoolSize>();

		for (auto &type : m_shaderProgram->GetDescriptors())
		{
			poolSizes.emplace_back(type.GetPoolSize());
		}

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();
		descriptorPoolCreateInfo.maxSets = 64; // Arbitrary number.

		vkDeviceWaitIdle(logicalDevice);
		Display::CheckVk(vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, allocator, &m_descriptorPool));
	}

	void Compute::CreatePipelineLayout()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;

		vkDeviceWaitIdle(logicalDevice);
		Display::CheckVk(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, allocator, &m_pipelineLayout));
	}

	void Compute::CreatePipelineCompute()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto pipelineCache = Renderer::Get()->GetVkPipelineCache();

		VkComputePipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.layout = m_pipelineLayout;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;
		pipelineCreateInfo.stage = m_shaderStageCreateInfo;

		vkCreateComputePipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, allocator, &m_pipeline);
	}
}
