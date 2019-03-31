#include "CommandBuffer.hpp"

#include "Renderer/Renderer.hpp"

namespace acid
{
CommandBuffer::CommandBuffer(const bool &begin, const VkQueueFlagBits &queueType, const VkCommandBufferLevel &bufferLevel) :
	m_commandPool(nullptr),
	m_queueType(queueType),
	m_commandBuffer(nullptr),
	m_running(false)
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	m_commandPool = Renderer::Get()->GetCommandPool();

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = m_commandPool->GetCommandPool();
	commandBufferAllocateInfo.level = bufferLevel;
	commandBufferAllocateInfo.commandBufferCount = 1;
	Renderer::CheckVk(vkAllocateCommandBuffers(logicalDevice->GetLogicalDevice(), &commandBufferAllocateInfo, &m_commandBuffer));

	if (begin)
	{
		Begin();
	}
}

CommandBuffer::~CommandBuffer()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	vkFreeCommandBuffers(logicalDevice->GetLogicalDevice(), m_commandPool->GetCommandPool(), 1, &m_commandBuffer);
}

void CommandBuffer::Begin(const VkCommandBufferUsageFlags &usage)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = usage;
	Renderer::CheckVk(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));

	m_running = true;
}

void CommandBuffer::End()
{
	Renderer::CheckVk(vkEndCommandBuffer(m_commandBuffer));

	m_running = false;
}

void CommandBuffer::SubmitIdle()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();
	auto queueSelected = GetQueue();

	if (m_running)
	{
		End();
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffer;

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	VkFence fence;
	Renderer::CheckVk(vkCreateFence(logicalDevice->GetLogicalDevice(), &fenceCreateInfo, nullptr, &fence));

	Renderer::CheckVk(vkResetFences(logicalDevice->GetLogicalDevice(), 1, &fence));

	Renderer::CheckVk(vkQueueSubmit(queueSelected, 1, &submitInfo, fence));

	//Renderer::CheckVk(vkQueueWaitIdle(queueSelected)); // Can be used instead of waiting for a fence.

	Renderer::CheckVk(vkWaitForFences(logicalDevice->GetLogicalDevice(), 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max()));

	vkDestroyFence(logicalDevice->GetLogicalDevice(), fence, nullptr);
}

void CommandBuffer::Submit(const VkSemaphore &waitSemaphore, const VkSemaphore &signalSemaphore, VkFence fence)
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();
	auto queueSelected = GetQueue();

	if (m_running)
	{
		End();
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffer;

	if (waitSemaphore != VK_NULL_HANDLE)
	{
		// Pipeline stages used to wait at for graphics queue submissions.
		static VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		submitInfo.pWaitDstStageMask = &submitPipelineStages;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &waitSemaphore;
	}

	if (signalSemaphore != VK_NULL_HANDLE)
	{
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &signalSemaphore;
	}

	// Before submitting to a queue wait for previous task to complete.
	// TODO: Use a global queue fence instead?
	Renderer::CheckVk(vkQueueWaitIdle(queueSelected));

	if (fence != VK_NULL_HANDLE)
	{
		Renderer::CheckVk(vkResetFences(logicalDevice->GetLogicalDevice(), 1, &fence));
		//Renderer::CheckVk(vkWaitForFences(logicalDevice->GetLogicalDevice(), 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max()));
	}

	Renderer::CheckVk(vkQueueSubmit(queueSelected, 1, &submitInfo, fence));
}

VkQueue CommandBuffer::GetQueue() const
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

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
