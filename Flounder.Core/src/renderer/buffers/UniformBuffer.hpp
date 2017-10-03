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
		UniformBuffer();

		UniformBuffer(const VkDeviceSize &size, const uint32_t &binding, const VkShaderStageFlags &stage);

		~UniformBuffer();

		void Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface);

		void Cleanup(const VkDevice &logicalDevice);

		void Update(const VkDevice &logicalDevice, void *newData);

		VkDeviceSize GetSize() const { return m_size; }

		void SetSize(const VkDeviceSize &size) { m_size = size; };

		uint32_t GetBinding() const { return m_binding; }

		void SetBinding(const uint32_t &binding) { m_binding = binding; }

		VkShaderStageFlags GetStage() const { return m_stage; }

		void SetStage(const VkShaderStageFlags &stage) { m_stage = stage; }

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorSetLayout; }
	};
}
