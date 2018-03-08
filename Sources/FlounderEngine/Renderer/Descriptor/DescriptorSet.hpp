#pragma once

#include <string>
#include "../../Engine/Platform.hpp"
#include "../Pipelines/PipelineCreate.hpp"

namespace Flounder
{
	class Pipeline;

	class F_EXPORT DescriptorSet
	{
	private:
		PipelineCreate m_pipelineCreateInfo;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;
		VkDescriptorSet m_descriptorSet;
	public:
		DescriptorSet(const PipelineCreate &pipelineCreateInfo);

		~DescriptorSet();

		void Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites);

		void BindDescriptor(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline);

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorSetLayout; }

		VkDescriptorPool GetDescriptorPool() const { return m_descriptorPool; }

		VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }

	private:
		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreateDescriptorSet();

	};
}
