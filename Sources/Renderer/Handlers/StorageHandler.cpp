#include "StorageHandler.hpp"

namespace acid
{
	StorageHandler::StorageHandler(const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(nullptr),
		m_size(0),
		m_data(nullptr),
		m_storageBuffer(nullptr),
		m_handlerStatus(HANDLER_STATUS_RESET)
	{
	}

	StorageHandler::StorageHandler(UniformBlock *uniformBlock, const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(uniformBlock),
		m_size(static_cast<uint32_t>(m_uniformBlock->GetSize())),
		m_data(std::make_unique<char[]>(m_size)),
		m_storageBuffer(std::make_unique<StorageBuffer>(static_cast<VkDeviceSize>(m_size))),
		m_handlerStatus(HANDLER_STATUS_CHANGED)
	{
	}

	bool StorageHandler::Update(UniformBlock *uniformBlock)
	{
		if (m_handlerStatus == HANDLER_STATUS_RESET || (m_multipipeline && m_uniformBlock == nullptr) || (!m_multipipeline && m_uniformBlock != uniformBlock))
		{
			if ((m_size == 0 && m_uniformBlock == nullptr) || (m_uniformBlock != nullptr && m_uniformBlock != uniformBlock && m_uniformBlock->GetSize() == m_size))
			{
				m_size = static_cast<uint32_t>(uniformBlock->GetSize());
			}

			m_uniformBlock = uniformBlock;
			m_data = std::make_unique<char[]>(m_size);
			m_storageBuffer = std::make_unique<StorageBuffer>(static_cast<VkDeviceSize>(m_size));
			m_handlerStatus = HANDLER_STATUS_CHANGED;
			return false;
		}

		if (m_handlerStatus != HANDLER_STATUS_NORMAL)
		{
			m_storageBuffer->Update(m_data.get());
			m_handlerStatus = HANDLER_STATUS_NORMAL;
		}

		return true;
	}
}
