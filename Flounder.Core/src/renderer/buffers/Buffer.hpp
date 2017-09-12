#pragma once

#include "Vertex.hpp"

namespace Flounder
{
	class Buffer
	{
	protected:
		VkBuffer m_buffer;
		VkDeviceMemory m_bufferMemory;
	public:
		Buffer();

		~Buffer();

		void Create(const VkDevice *logicalDevice, const VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

		void Cleanup(const VkDevice *logicalDevice);

		VkBuffer GetBuffer() const { return m_buffer; }

		VkDeviceMemory GetBufferMemory() const { return m_bufferMemory; }
	protected:
		uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	};
}
