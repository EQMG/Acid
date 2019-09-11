#include "StorageHandler.hpp"

namespace acid
{
StorageHandler::StorageHandler(bool multipipeline) :
	m_multipipeline{multipipeline},
	m_handlerStatus{Buffer::Status::Reset}
{
}

StorageHandler::StorageHandler(const Shader::UniformBlock &uniformBlock, bool multipipeline) :
	m_multipipeline{multipipeline},
	m_uniformBlock{uniformBlock},
	m_size{static_cast<uint32_t>(m_uniformBlock->GetSize())},
	m_storageBuffer{std::make_unique<StorageBuffer>(static_cast<VkDeviceSize>(m_size))},
	m_handlerStatus{Buffer::Status::Changed}
{
}

bool StorageHandler::Update(const std::optional<Shader::UniformBlock> &uniformBlock)
{
	if (m_handlerStatus == Buffer::Status::Reset || (m_multipipeline && !m_uniformBlock) || (!m_multipipeline && m_uniformBlock != uniformBlock))
	{
		if ((m_size == 0 && !m_uniformBlock) || (m_uniformBlock && m_uniformBlock != uniformBlock && static_cast<uint32_t>(m_uniformBlock->GetSize()) == m_size))
		{
			m_size = static_cast<uint32_t>(uniformBlock->GetSize());
		}

		m_uniformBlock = uniformBlock;
		m_bound = false;
		m_storageBuffer = std::make_unique<StorageBuffer>(static_cast<VkDeviceSize>(m_size));
		m_handlerStatus = Buffer::Status::Changed;
		return false;
	}

	if (m_handlerStatus != Buffer::Status::Normal)
	{
		if (m_bound)
		{
			m_storageBuffer->UnmapMemory();
			m_bound = false;
		}

		m_handlerStatus = Buffer::Status::Normal;
	}

	return true;
}
}
