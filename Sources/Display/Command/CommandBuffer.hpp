#pragma once

#include <vulkan/vulkan.h>
#include "Engine/Exports.hpp"

namespace acid
{
	enum CommandBufferLevel
	{
		COMMAND_BUFFER_LEVEL_PRIMARY = 0,
		COMMAND_BUFFER_LEVEL_SECONDARY = 1
	};

	enum CommandQueueType
	{
		COMMAND_QUEUE_GRAPHICS = 0,
		COMMAND_QUEUE_COMPUTE = 1
	};

	class ACID_EXPORT CommandBuffer
	{
	private:
		CommandBufferLevel m_bufferLevel;
		CommandQueueType m_queueType;
		VkCommandBuffer m_commandBuffer;
	public:
		CommandBuffer(const bool &begin = true, const CommandBufferLevel &bufferLevel = CommandBufferLevel::COMMAND_BUFFER_LEVEL_PRIMARY, const CommandQueueType &queueType = COMMAND_QUEUE_GRAPHICS);

		~CommandBuffer();

		void Begin();

		void End();

		void Submit();

		VkCommandBuffer GetVkCommandBuffer() const { return m_commandBuffer; }
	};
}
