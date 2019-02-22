#include "InstanceBuffer.hpp"

#include "Renderer/Renderer.hpp"

namespace acid
{
	InstanceBuffer::InstanceBuffer(const VkDeviceSize &size) :
		Buffer(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
	{
	}

	void InstanceBuffer::Map(const CommandBuffer &commandBuffer, void **data)
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		Renderer::CheckVk(vkMapMemory(logicalDevice->GetLogicalDevice(), GetBufferMemory(), 0, m_size, 0, data));
	}

	void InstanceBuffer::Unmap(const CommandBuffer &commandBuffer)
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), GetBufferMemory());
	}

	void InstanceBuffer::Update(const CommandBuffer &commandBuffer, const void *newData)
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		// Copies the data to the buffer.
		void *data;
		Renderer::CheckVk(vkMapMemory(logicalDevice->GetLogicalDevice(), GetBufferMemory(), 0, m_size, 0, &data));
		std::memcpy(data, newData, static_cast<std::size_t>(m_size));
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), GetBufferMemory());
	}
}
