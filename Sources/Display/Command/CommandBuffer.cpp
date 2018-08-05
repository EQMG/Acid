#include "CommandBuffer.hpp"

#include "Display/Display.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	CommandBuffer::CommandBuffer(const bool &begin, const CommandBufferLevel &bufferLevel, const CommandQueueType &queueType) :
		m_bufferLevel(bufferLevel),
		m_queueType(queueType),
		m_commandBuffer(VK_NULL_HANDLE)
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto commandPool = Renderer::Get()->GetVkCommandPool();

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.level = bufferLevel == COMMAND_BUFFER_LEVEL_PRIMARY ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		commandBufferAllocateInfo.commandBufferCount = 1;

		Display::CheckVk(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &m_commandBuffer));

		if (begin)
		{
			Begin();
		}
	}

	CommandBuffer::~CommandBuffer()
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto commandPool = Renderer::Get()->GetVkCommandPool();

		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &m_commandBuffer);
	}

	void CommandBuffer::Begin()
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		Display::CheckVk(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
	}

	void CommandBuffer::End()
	{
		Display::CheckVk(vkEndCommandBuffer(m_commandBuffer));
	}

	void CommandBuffer::Submit()
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto queueGraphics = m_queueType == COMMAND_QUEUE_GRAPHICS ? Display::Get()->GetVkQueueGraphics() : Display::Get()->GetVkQueueCompute(); // TODO: Clean up more.

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffer;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = 0;

		VkFence fence;
		Display::CheckVk(vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &fence));

		Display::CheckVk(vkQueueSubmit(queueGraphics, 1, &submitInfo, fence));

		Display::CheckVk(vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE, UINT64_MAX));

		vkDestroyFence(logicalDevice, fence, nullptr);
	}
}
