#pragma once

#include <vector>

#include "Buffer.hpp"

namespace Flounder
{
	class IndexBuffer :
		public Buffer
	{
	private:
		std::vector<uint16_t> m_indices;
	public:
		IndexBuffer();

		IndexBuffer(const std::vector<uint16_t> &vertices);

		~IndexBuffer();

		void Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkQueue &queue, const VkCommandPool &transferCommandPool);

		void Cleanup(const VkDevice &logicalDevice);

		void SetIndices(std::vector<uint16_t> indices);

		uint32_t GetIndicesSize() const { return static_cast<uint32_t>(m_indices.size()); }
	private:
		void CopyVerticesToBuffer(const VkDevice &logicalDevice, const VkDeviceSize &bufferSize, Buffer &bufferStaging) const;
	};
}
