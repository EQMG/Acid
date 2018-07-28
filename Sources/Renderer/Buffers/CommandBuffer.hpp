#pragma once

#include "Display/Display.hpp"

namespace acid
{
	enum CommandBufferLevel
	{
		BUFFER_LEVEL_PRIMARY = 0,
		BUFFER_LEVEL_SECONDARY = 1
	};

	class ACID_EXPORT CommandBuffer
	{
	private:
		CommandBufferLevel m_bufferLevel;
		VkCommandBuffer m_commandBuffer;
	public:
		CommandBuffer(const bool &begin = true, const CommandBufferLevel &bufferLevel = CommandBufferLevel::BUFFER_LEVEL_PRIMARY);

		~CommandBuffer();

		void Begin();

		VkCommandBuffer GetVkCommandBuffer() const { return m_commandBuffer; }
	};
}
