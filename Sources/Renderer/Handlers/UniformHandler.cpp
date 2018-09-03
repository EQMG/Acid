#include "UniformHandler.hpp"

namespace acid
{
	UniformHandler::UniformHandler(const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(nullptr),
		m_uniformBuffer(nullptr),
		m_data(nullptr),
		m_changed(false)
	{
	}

	UniformHandler::UniformHandler(const std::shared_ptr<UniformBlock> &uniformBlock, const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(uniformBlock),
		m_uniformBuffer(std::make_shared<UniformBuffer>(static_cast<VkDeviceSize>(m_uniformBlock->GetSize()))),
		m_data(malloc(static_cast<size_t>(m_uniformBlock->GetSize()))),
		m_changed(true)
	{
	}

	UniformHandler::~UniformHandler()
	{
		free(m_data);
	}

	bool UniformHandler::Update(const std::shared_ptr<UniformBlock> &uniformBlock)
	{
		if ((m_multipipeline && m_uniformBlock == nullptr) || (!m_multipipeline && m_uniformBlock != uniformBlock))
		{
			free(m_data);

			m_uniformBlock = uniformBlock;
			m_uniformBuffer = std::make_shared<UniformBuffer>(static_cast<VkDeviceSize>(m_uniformBlock->GetSize()));
			m_data = malloc(static_cast<size_t>(m_uniformBlock->GetSize()));
			m_changed = false;
			return false;
		}

		if (m_changed)
		{
			m_uniformBuffer->Update(m_data);
			m_changed = false;
		}

		return true;
	}
}
