#pragma once

#include "CommandPool.hpp"

namespace acid
{
class ACID_EXPORT CommandBuffer
{
public:
	/// <summary>
	/// Creates a new command buffer.
	/// </summary>
	/// <param name="begin"> If recording will start right away, if true <seealso cref="#Begin()"/> is called. </param>
	/// <param name="queueType"> The queue to run this command buffer on. </param>
	/// <param name="bufferLevel"> The buffer level. </param>
	explicit CommandBuffer(const bool &begin = true, const VkQueueFlagBits &queueType = VK_QUEUE_GRAPHICS_BIT,
		const VkCommandBufferLevel &bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	~CommandBuffer();

	/// <summary>
	/// Begins the recording state for this command buffer.
	/// </summary>
	/// <param name="usage"> How this command buffer will be used. </param>
	void Begin(const VkCommandBufferUsageFlags &usage = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	/// <summary>
	/// Ends the recording state for this command buffer.
	/// </summary>
	void End();

	/// <summary>
	/// Submits the command buffer to the queue and will hold the current thread idle until it has finished.
	/// </summary>
	void SubmitIdle();

	/// <summary>
	/// Submits the command buffer.
	/// </summary>
	/// <param name="waitSemaphore"> A optional semaphore that will waited upon before the command buffer is executed. </param>
	/// <param name="signalSemaphore"> A optional that is signaled once the command buffer has been excecuted. </param>
	/// <param name="fence"> A optional fence that is signaled once the command buffer has completed. </param>
	void Submit(const VkSemaphore &waitSemaphore = VK_NULL_HANDLE, const VkSemaphore &signalSemaphore = VK_NULL_HANDLE, VkFence fence = VK_NULL_HANDLE);

	const bool &IsRunning() const { return m_running; }

	const VkCommandBuffer &GetCommandBuffer() const { return m_commandBuffer; }

private:
	VkQueue GetQueue() const;

	std::shared_ptr<CommandPool> m_commandPool;

	VkQueueFlagBits m_queueType;
	VkCommandBuffer m_commandBuffer;
	bool m_running;
};
}
