#include "UniformBuffer.hpp"

#include "../../devices/Display.hpp"

namespace Flounder
{
	UniformBuffer::UniformBuffer(const VkDeviceSize &size, const uint32_t &binding, const VkShaderStageFlags &stage) :
		Buffer(),
		m_size(size),
		m_binding(binding),
		m_stage(stage)
	{
	}

	UniformBuffer::~UniformBuffer()
	{
	}

	void UniformBuffer::Create()
	{
		// Creates the buffer.
		Buffer::Create(m_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	}

	void UniformBuffer::Cleanup()
	{
		Buffer::Cleanup();
	}

	void UniformBuffer::Update(void *newData)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		// Copies the data to the buffer.
		void *data;
		vkMapMemory(logicalDevice, m_bufferMemory, 0, m_size, 0, &data);
		memcpy(data, newData, m_size);
		vkUnmapMemory(logicalDevice, m_bufferMemory);
	}
}
