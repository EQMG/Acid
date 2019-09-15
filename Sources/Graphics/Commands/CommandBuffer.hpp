#pragma once

#include "CommandPool.hpp"

namespace acid {
/**
 * @brief Class that represents a command buffer.
 */
class ACID_EXPORT
CommandBuffer
{
public:
	/**
	 * Creates a new command buffer.
	 * @param begin If recording will start right away, if true {@link CommandBuffer#Begin} is called.
	 * @param queueType The queue to run this command buffer on.
	 * @param bufferLevel The buffer level.
	 */
	explicit CommandBuffer(bool begin = true, const VkQueueFlagBits &queueType = VK_QUEUE_GRAPHICS_BIT,
		const VkCommandBufferLevel &bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	~CommandBuffer();

	/**
	 * Begins the recording state for this command buffer.
	 * @param usage How this command buffer will be used.
	 */
	void Begin(const VkCommandBufferUsageFlags &usage = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	/**
	 * Ends the recording state for this command buffer.
	 */
	void End();

	/**
	 * Submits the command buffer to the queue and will hold the current thread idle until it has finished.
	 */
	void SubmitIdle();

	/**
	 * Submits the command buffer.
	 * @param waitSemaphore A optional semaphore that will waited upon before the command buffer is executed.
	 * @param signalSemaphore A optional that is signaled once the command buffer has been executed.
	 * @param fence A optional fence that is signaled once the command buffer has completed.
	 */
	void Submit(const VkSemaphore &waitSemaphore = VK_NULL_HANDLE, const VkSemaphore &signalSemaphore = VK_NULL_HANDLE, VkFence fence = VK_NULL_HANDLE);

	bool IsRunning() const { return m_running; }

	operator const VkCommandBuffer &() const { return m_commandBuffer; }

	const VkCommandBuffer &GetCommandBuffer() const { return m_commandBuffer; }

private:
	VkQueue GetQueue() const;

	std::shared_ptr<CommandPool> m_commandPool;

	VkQueueFlagBits m_queueType;
	VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
	bool m_running = false;
};
}
