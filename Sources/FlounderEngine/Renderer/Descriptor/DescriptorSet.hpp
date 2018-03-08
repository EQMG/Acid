#pragma once

#include <string>
#include "../../Engine/Platform.hpp"
#include "../Pipelines/PipelineCreate.hpp"

namespace Flounder
{
	class Pipeline;

	class DescriptorSet
	{
	private:
		std::vector<DescriptorType> m_descriptors;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;
		VkDescriptorSet m_descriptorSet;
	public:
		DescriptorSet(const std::vector<DescriptorType> &descriptors);

		~DescriptorSet();

		void Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites);

		void BindDescriptor(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline);

		VkDescriptorSetLayout GetDescriptorSetLayout() { return m_descriptorSetLayout; }

		VkDescriptorPool GetDescriptorPool() const { return m_descriptorPool; }

		VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }

	private:
		void CreateDescriptorLayout();

		void CreateDescriptorPool();

		void CreateDescriptorSet();
	};
}
