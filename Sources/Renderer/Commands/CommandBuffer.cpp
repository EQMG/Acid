#include "CommandBuffer.hpp"

#include "Display/Display.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	CommandBuffer::CommandBuffer(const bool &begin, const VkQueueFlagBits &queueType, const VkCommandBufferLevel &bufferLevel) :
		m_queueType(queueType),
		m_bufferLevel(bufferLevel),
		m_commandBuffer(VK_NULL_HANDLE)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto commandPool = Renderer::Get()->GetCommandPool();

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.level = bufferLevel;
		commandBufferAllocateInfo.commandBufferCount = 1;

		Display::CheckVk(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &m_commandBuffer));

		if (begin)
		{
			Begin();
		}
	}

	CommandBuffer::~CommandBuffer()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto commandPool = Renderer::Get()->GetCommandPool();

		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &m_commandBuffer);
	}

	void CommandBuffer::Begin(const VkCommandBufferUsageFlags &usage) const
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usage;

		Display::CheckVk(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
	}

	void CommandBuffer::End() const
	{
		Display::CheckVk(vkEndCommandBuffer(m_commandBuffer));
	}

	void CommandBuffer::Submit(const bool &waitFence, const VkSemaphore &semaphore) const
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto queueSelected = GetQueue();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffer;

		if (semaphore != VK_NULL_HANDLE)
		{
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &semaphore;
		}

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = 0;

		VkFence fence = VK_NULL_HANDLE;

		if (waitFence)
		{
			Display::CheckVk(vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &fence));
		}

		Display::CheckVk(vkQueueSubmit(queueSelected, 1, &submitInfo, fence));

		if (waitFence)
		{
			Display::CheckVk(vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE, UINT64_MAX));

			vkDestroyFence(logicalDevice, fence, nullptr);
		}
	}

	VkQueue CommandBuffer::GetQueue() const
	{
		switch (m_queueType)
		{
		case VK_QUEUE_GRAPHICS_BIT:
			return Display::Get()->GetGraphicsQueue();
		case VK_QUEUE_COMPUTE_BIT:
			return Display::Get()->GetComputeQueue();
		default:
			return nullptr;
		}
	}
}
