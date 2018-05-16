#pragma once

#include "Display/Display.hpp"

namespace fl
{
	enum CommandBufferLevel
	{
		BUFFER_LEVEL_PRIMARY = 0,
		BUFFER_LEVEL_SECONDARY = 1
	};

	class FL_EXPORT CommandBuffer
	{
	private:
		CommandBufferLevel m_bufferLevel;
		VkCommandBuffer m_commandBuffer;
	public:
		CommandBuffer(const bool &begin = true, const CommandBufferLevel &bufferLevel = CommandBufferLevel::BUFFER_LEVEL_PRIMARY);

		~CommandBuffer();

		void Begin();

		FL_HIDDEN VkCommandBuffer GetVkCommandBuffer() const { return m_commandBuffer; }
	};
}
