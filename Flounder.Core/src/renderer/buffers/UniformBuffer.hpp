#pragma once

#include <vector>

#include "Buffer.hpp"

namespace Flounder
{
	class UniformBuffer :
		public Buffer
	{
	private:
		VkDeviceSize m_size;
		uint32_t m_binding;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;
		VkDescriptorSet m_descriptorSet;
	public:
		UniformBuffer();

		UniformBuffer(const VkDeviceSize &size, const uint32_t &binding);

		~UniformBuffer();

		void Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface);

		void Cleanup(const VkDevice &logicalDevice);

		void Update(const VkDevice &logicalDevice, void *newData);

		VkDeviceSize GetSize() const { return m_size; }

		void SetSize(const VkDeviceSize &size) { m_size = size; };

		uint32_t GetBinding() const { return m_binding; }

		void SetBinding(const uint32_t &binding) { m_binding = binding; }

		VkDescriptorSetLayout GetDescriptorSetLayout() { return m_descriptorSetLayout; }

		VkDescriptorPool GetDescriptorPool() { return m_descriptorPool; }

		VkDescriptorSet GetDescriptorSet() { return m_descriptorSet; }
	};
}
