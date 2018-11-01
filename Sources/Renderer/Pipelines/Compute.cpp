#include "Compute.hpp"

#include <cmath>
#include <cassert>
#include "Display/Display.hpp"
#include "Helpers/FileSystem.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	Compute::Compute(const ComputeCreate &computeCreate) :
		IPipeline(),
		m_computeCreate(computeCreate),
		m_shaderProgram(std::make_unique<ShaderProgram>(m_computeCreate.GetShaderStage())),
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

		CreateShaderProgram();
		CreateDescriptorLayout();
		CreateDescriptorPool();
		CreatePipelineLayout();
		CreatePipelineCompute();

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
	//	Log::Out("%s", m_shaderProgram->ToString().c_str());
		Log::Out("Compute pipeline '%s' created in %ims\n", m_computeCreate.GetShaderStage().c_str(), (debugEnd - debugStart).AsMilliseconds());
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
		uint32_t groupCountX = static_cast<uint32_t>(std::ceil(static_cast<float>(m_computeCreate.GetWidth()) / static_cast<float>(m_computeCreate.GetWorkgroupSize())));
		uint32_t groupCountY = static_cast<uint32_t>(std::ceil(static_cast<float>(m_computeCreate.GetHeight()) / static_cast<float>(m_computeCreate.GetWorkgroupSize())));
		vkCmdDispatch(commandBuffer.GetCommandBuffer(), groupCountX, groupCountY, 1);
		return true;
	}

	void Compute::CreateShaderProgram()
	{
		std::stringstream defineBlock;
		defineBlock << "\n";

		for (auto &define : m_computeCreate.GetDefines())
		{
			defineBlock << "#define " << define.GetName() << " " << define.GetValue() << "\n";
		}

		auto fileLoaded = Files::Read(m_computeCreate.GetShaderStage());

		if (!fileLoaded)
		{
			Log::Error("Shader Stage could not be loaded: '%s'\n", m_computeCreate.GetShaderStage().c_str());
			assert(false && "Could not create compute pipeline, missing shader stage!");
			return;
		}

		auto shaderCode = ShaderProgram::InsertDefineBlock(*fileLoaded, defineBlock.str());
		shaderCode = ShaderProgram::ProcessIncludes(shaderCode);

		VkShaderStageFlagBits stageFlag = ShaderProgram::GetShaderStage(m_computeCreate.GetShaderStage());
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
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		std::vector<VkDescriptorSetLayoutBinding> bindings = std::vector<VkDescriptorSetLayoutBinding>();

		for (auto &type : m_shaderProgram->GetDescriptors())
		{
			bindings.emplace_back(type.GetLayoutBinding());
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		descriptorSetLayoutCreateInfo.pBindings = bindings.data();

		Display::CheckVk(vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
	}

	void Compute::CreateDescriptorPool()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

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
		descriptorPoolCreateInfo.maxSets = 256; // Arbitrary number.

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
		pipelineCreateInfo.layout = m_pipelineLayout;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;
		pipelineCreateInfo.stage = m_shaderStageCreateInfo;

		vkCreateComputePipelines(logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline);
	}
}
