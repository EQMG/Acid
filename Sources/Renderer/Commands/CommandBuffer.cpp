#include "CommandBuffer.hpp"

#include "Devices/Window.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	CommandBuffer::CommandBuffer(const bool &begin, const VkQueueFlagBits &queueType, const VkCommandBufferLevel &bufferLevel) :
		m_queueType(queueType),
		m_bufferLevel(bufferLevel),
		m_commandBuffer(VK_NULL_HANDLE),
		m_running(false)
	{
		auto logicalDevice = Window::Get()->GetLogicalDevice();
		auto commandPool = Renderer::Get()->GetCommandPool();

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.level = bufferLevel;
		commandBufferAllocateInfo.commandBufferCount = 1;
		Window::CheckVk(vkAllocateCommandBuffers(logicalDevice->GetLogicalDevice(), &commandBufferAllocateInfo, &m_commandBuffer));

		if (begin)
		{
			Begin();
		}
	}

	CommandBuffer::~CommandBuffer()
	{
		auto logicalDevice = Window::Get()->GetLogicalDevice();
		auto commandPool = Renderer::Get()->GetCommandPool();

		vkFreeCommandBuffers(logicalDevice->GetLogicalDevice(), commandPool, 1, &m_commandBuffer);
	}

	void CommandBuffer::Begin(const VkCommandBufferUsageFlags &usage)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usage;
		Window::CheckVk(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));

		m_running = true;
	}

	void CommandBuffer::End()
	{
		Window::CheckVk(vkEndCommandBuffer(m_commandBuffer));

		m_running = false;
	}

	void CommandBuffer::Submit(VkSemaphore signalSemaphore, VkFence fence, const bool &createFence)
	{
		auto logicalDevice = Window::Get()->GetLogicalDevice();
		auto queueSelected = GetQueue();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffer;

		if (signalSemaphore != VK_NULL_HANDLE)
		{
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &signalSemaphore;
		}

		bool createdFence = false;

		if (fence == VK_NULL_HANDLE && createFence)
		{
			VkFenceCreateInfo fenceCreateInfo = {};
			fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceCreateInfo.flags = 0;
			Window::CheckVk(vkCreateFence(logicalDevice->GetLogicalDevice(), &fenceCreateInfo, nullptr, &fence));

			createdFence = true;
		}

		if (fence != VK_NULL_HANDLE)
		{
			Window::CheckVk(vkResetFences(logicalDevice->GetLogicalDevice(), 1, &fence));
		}

		Window::CheckVk(vkQueueSubmit(queueSelected, 1, &submitInfo, fence));

		if (fence != VK_NULL_HANDLE)
		{
			Window::CheckVk(vkWaitForFences(logicalDevice->GetLogicalDevice(), 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max()));

			if (createdFence)
			{
				vkDestroyFence(logicalDevice->GetLogicalDevice(), fence, nullptr);
			}
		}
	}

	VkQueue CommandBuffer::GetQueue() const
	{
		auto logicalDevice = Window::Get()->GetLogicalDevice();

		switch (m_queueType)
		{
		case VK_QUEUE_GRAPHICS_BIT:
			return logicalDevice->GetGraphicsQueue();
		case VK_QUEUE_COMPUTE_BIT:
			return logicalDevice->GetComputeQueue();
		default:
			return nullptr;
		}
	}
}
