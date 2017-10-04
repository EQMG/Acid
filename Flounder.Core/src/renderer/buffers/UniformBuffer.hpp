#pragma once

#include "Buffer.hpp"

#include "../pipelines/Descriptor.hpp"

namespace Flounder
{
	class UniformBuffer :
		public Buffer, 
		public Descriptor
	{
	private:
		VkDeviceSize m_size;
		VkShaderStageFlags m_stage;
	public:
		UniformBuffer(const VkDeviceSize &size, const VkShaderStageFlags &stage);

		~UniformBuffer();

		void Create();

		void Cleanup();

		void Update(void *newData);

		VkDescriptorSetLayoutBinding GetDescriptorLayout(const uint32_t &binding) override;

		VkDescriptorPoolSize GetDescriptorPool(const uint32_t &binding) override;

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorSet &descriptorSet) override;

		VkDeviceSize GetSize() const { return m_size; }

		VkShaderStageFlags GetStage() const { return m_stage; }
	};
}
