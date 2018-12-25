#include "UniformHandler.hpp"

namespace acid
{
	UniformHandler::UniformHandler(const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(nullptr),
		m_size(0),
		m_data(nullptr),
		m_uniformBuffer(nullptr),
		m_handlerStatus(HANDLER_STATUS_NORMAL)
	{
	}

	UniformHandler::UniformHandler(UniformBlock *uniformBlock, const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(uniformBlock),
		m_size(static_cast<uint32_t>(m_uniformBlock->GetSize())),
		m_data(std::make_unique<char[]>(m_size)),
		m_uniformBuffer(std::make_unique<UniformBuffer>(static_cast<VkDeviceSize>(m_size))),
		m_handlerStatus(HANDLER_STATUS_NORMAL)
	{
	}

	bool UniformHandler::Update(UniformBlock *uniformBlock)
	{
		if (m_handlerStatus == HANDLER_STATUS_RESET || (m_multipipeline && m_uniformBlock == nullptr) || (!m_multipipeline && m_uniformBlock != uniformBlock))
		{
			if ((m_size == 0 && m_uniformBlock == nullptr) || (m_uniformBlock != nullptr && m_uniformBlock != uniformBlock && m_uniformBlock->GetSize() == m_size))
			{
				m_size = static_cast<uint32_t>(uniformBlock->GetSize());
			}

			m_uniformBlock = uniformBlock;
			m_data = std::make_unique<char[]>(m_size);
			m_uniformBuffer = std::make_unique<UniformBuffer>(static_cast<VkDeviceSize>(m_size));
			m_handlerStatus = HANDLER_STATUS_CHANGED;
			return false;
		}

		if (m_handlerStatus != HANDLER_STATUS_NORMAL)
		{
			m_uniformBuffer->Update(m_data.get());
			m_handlerStatus = HANDLER_STATUS_NORMAL;
		}

		return true;
	}
}
