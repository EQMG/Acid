#pragma once

#include "Display/Display.hpp"

namespace fl
{
	enum CommandBufferLevel
	{
		BufferLevelPrimary = 0,
		BufferLevelSecondary = 1
	};

	class FL_EXPORT CommandBuffer
	{
	private:
		CommandBufferLevel m_bufferLevel;
		VkCommandBuffer m_commandBuffer;
	public:
		CommandBuffer(const bool &begin = true, const CommandBufferLevel &bufferLevel = CommandBufferLevel::BufferLevelPrimary);

		~CommandBuffer();

		void Begin();

		FL_HIDDEN VkCommandBuffer GetVkCommandBuffer() const { return m_commandBuffer; }
	};
}
