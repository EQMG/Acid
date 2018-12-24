#include "UniformHandler.hpp"

namespace acid
{
	UniformHandler::UniformHandler(const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(nullptr),
		m_offsetSize(OffsetSize(0, 0)),
		m_data(nullptr),
		m_uniformBuffer(nullptr),
		m_changed(false)
	{
	}

	UniformHandler::UniformHandler(UniformBlock *uniformBlock, const bool &multipipeline, const std::optional<OffsetSize> &offsetSize) :
		m_multipipeline(multipipeline),
		m_uniformBlock(uniformBlock),
		m_offsetSize(offsetSize ? *offsetSize : OffsetSize(0, m_uniformBlock->GetSize())),
		m_data(malloc(static_cast<size_t>(m_offsetSize.GetSize()))),
		m_uniformBuffer(std::make_unique<UniformBuffer>(static_cast<VkDeviceSize>(m_offsetSize.GetSize()))),
		m_changed(true)
	{
	}

	UniformHandler::~UniformHandler()
	{
		free(m_data);
	}

	bool UniformHandler::Update(UniformBlock *uniformBlock, const std::optional<OffsetSize> &offsetSize)
	{
		if ((m_multipipeline && m_uniformBlock == nullptr) || (!m_multipipeline && m_uniformBlock != uniformBlock))
		{
			free(m_data);

			m_uniformBlock = uniformBlock;

			if (offsetSize)
			{
				m_offsetSize = *offsetSize;
			}
			else
			{
				m_offsetSize = OffsetSize(0, static_cast<uint32_t>(m_uniformBlock->GetSize()));
			}

			m_data = malloc(static_cast<size_t>(m_offsetSize.GetSize()));
			m_uniformBuffer = std::make_unique<UniformBuffer>(static_cast<VkDeviceSize>(m_offsetSize.GetSize()));
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
