#pragma once

#include <iostream>
#include <vector>

#include "Buffer.hpp"

namespace Flounder
{
	class VertexBuffer :
		public Buffer
	{
	private:
		const std::vector<Vertex> vertices =
		{
			{ { 0.0f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
			{ { 0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f } },
			{ { -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } }
		};
	public:
		VertexBuffer();

		~VertexBuffer();

		void Create(const VkDevice *logicalDevice, const VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, VkCommandPool *transferCommandPool, const VkQueue *transferQueue);

		void Cleanup(const VkDevice *logicalDevice);

		uint32_t GetVerticesSize() const { return static_cast<uint32_t>(vertices.size()); }
	private:
		void CopyVerticesToBuffer(const VkDevice *logicalDevice, const VkDeviceSize &bufferSize, const Buffer &bufferStaging) const;

		void CopyBuffer(const VkDevice *logicalDevice, const VkCommandPool *transferCommandPool,
			VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const VkQueue* transferQueue) const;
	};
}
