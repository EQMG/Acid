#pragma once

#include "Buffer.hpp"

namespace Flounder
{
	class UniformBuffer :
		public Buffer
	{
	private:
		VkDeviceSize m_size;
		uint32_t m_binding;
		VkShaderStageFlags m_stage;

		VkDescriptorSetLayout m_descriptorSetLayout;
	public:
		UniformBuffer(const VkDeviceSize &size, const uint32_t &binding, const VkShaderStageFlags &stage);

		~UniformBuffer();

		void Create();

		void Cleanup();

		void Update(void *newData);

		VkDeviceSize GetSize() const { return m_size; }

		uint32_t GetBinding() const { return m_binding; }

		VkShaderStageFlags GetStage() const { return m_stage; }

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorSetLayout; }
	};
}
