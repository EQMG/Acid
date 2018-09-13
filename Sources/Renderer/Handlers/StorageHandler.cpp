#include "StorageHandler.hpp"

namespace acid
{
	StorageHandler::StorageHandler(const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(nullptr),
		m_storageBuffer(nullptr),
		m_data(nullptr),
		m_changed(false)
	{
	}

	StorageHandler::StorageHandler(UniformBlock *uniformBlock, const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(uniformBlock),
		m_storageBuffer(std::make_unique<StorageBuffer>(static_cast<VkDeviceSize>(m_uniformBlock->GetSize()))),
		m_data(malloc(static_cast<size_t>(m_uniformBlock->GetSize()))),
		m_changed(true)
	{
	}

	StorageHandler::~StorageHandler()
	{
		free(m_data);
	}

	bool StorageHandler::Update(UniformBlock *uniformBlock)
	{
		if ((m_multipipeline && m_uniformBlock == nullptr) || (!m_multipipeline && m_uniformBlock != uniformBlock))
		{
			free(m_data);

			m_uniformBlock = uniformBlock;
			m_storageBuffer = std::make_unique<StorageBuffer>(static_cast<VkDeviceSize>(m_uniformBlock->GetSize()));
			m_data = malloc(static_cast<size_t>(m_uniformBlock->GetSize()));
			m_changed = false;
			return false;
		}

		if (m_changed)
		{
			m_storageBuffer->Update(m_data);
			m_changed = false;
		}

		return true;
	}
}
