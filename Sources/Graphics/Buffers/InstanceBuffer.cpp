#include "InstanceBuffer.hpp"

#include "Graphics/Graphics.hpp"

namespace acid
{
InstanceBuffer::InstanceBuffer(const VkDeviceSize &size) :
	Buffer(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
{
}

void InstanceBuffer::Update(const CommandBuffer &commandBuffer, const void *newData)
{
	void *data;
	Buffer::MapMemory(&data);
	std::memcpy(data, newData, static_cast<std::size_t>(m_size));
	Buffer::UnmapMemory();
}
}
