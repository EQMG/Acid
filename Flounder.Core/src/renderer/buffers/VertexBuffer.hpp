#pragma once

#include <vector>

#include "Buffer.hpp"

namespace Flounder
{
	class VertexBuffer :
		public Buffer
	{
	private:
		std::vector<Vertex> m_vertices;
	public:
		VertexBuffer();

		VertexBuffer(const std::vector<Vertex> &vertices);

		~VertexBuffer();

		void Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkQueue &queue, const VkCommandPool &transferCommandPool);

		void Cleanup(const VkDevice &logicalDevice);

		void SetVerticies(std::vector<Vertex> &vertices);

		uint32_t GetVerticesSize() const { return static_cast<uint32_t>(m_vertices.size()); }
	private:
		void CopyVerticesToBuffer(const VkDevice &logicalDevice, const VkDeviceSize &bufferSize, Buffer &bufferStaging) const;

		void CopyBuffer(const VkDevice &logicalDevice, const VkQueue &queue, const VkCommandPool &transferCommandPool, const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize &size);
	};
}
