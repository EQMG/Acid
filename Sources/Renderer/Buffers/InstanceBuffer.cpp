#include "InstanceBuffer.hpp"

#include "Display/Display.hpp"

namespace acid
{
	InstanceBuffer::InstanceBuffer(const VkDeviceSize &size) :
		Buffer(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
	{
	}

	void InstanceBuffer::Update(const void *newData)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		// Copies the data to the buffer.
		void *data;
		vkMapMemory(logicalDevice, m_bufferMemory, 0, m_size, 0, &data);
		memcpy(data, newData, static_cast<size_t>(m_size));
		vkUnmapMemory(logicalDevice, m_bufferMemory);
	}
}
