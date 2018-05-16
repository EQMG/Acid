#include "CommandBuffer.hpp"

#include "Renderer/Renderer.hpp"

namespace fl
{
	CommandBuffer::CommandBuffer(const bool &begin, const CommandBufferLevel &bufferLevel) :
		m_bufferLevel(bufferLevel),
		m_commandBuffer(VK_NULL_HANDLE)
	{
		const auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		const auto commandPool = Renderer::Get()->GetVkCommandPool();

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.commandBufferCount = 1;
		commandBufferAllocateInfo.level = bufferLevel == BufferLevelPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;

		Display::ErrorVk(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &m_commandBuffer));

		if (begin)
		{
			Begin();
		}
	}

	CommandBuffer::~CommandBuffer()
	{
		const auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		const auto queue = Display::Get()->GetVkQueue();
		const auto commandPool = Renderer::Get()->GetVkCommandPool();

		Display::ErrorVk(vkEndCommandBuffer(m_commandBuffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffer;

		Display::ErrorVk(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

		Display::ErrorVk(vkQueueWaitIdle(queue));

		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &m_commandBuffer);
	}

	void CommandBuffer::Begin()
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		Display::ErrorVk(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
	}
}
