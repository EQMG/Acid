#include "PushHandler.hpp"

namespace acid
{
	PushHandler::PushHandler(const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(nullptr),
		m_offsetSize(OffsetSize(0, 0)),
		m_data(nullptr)
	{
	}

	PushHandler::PushHandler(UniformBlock *uniformBlock, const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(uniformBlock),
		m_offsetSize(OffsetSize(0, 0)),
		m_data(malloc(static_cast<size_t>(m_uniformBlock->GetSize())))
	{
	}

	PushHandler::~PushHandler()
	{
		free(m_data);
	}

	bool PushHandler::Update(UniformBlock *uniformBlock, const std::optional<OffsetSize> &offsetSize)
	{
		if ((m_multipipeline && m_uniformBlock == nullptr) || (!m_multipipeline && m_uniformBlock != uniformBlock))
		{
			free(m_data);

			//if (offsetSize)
			//{
			//	m_offsetSize = *offsetSize;
			//}
			//else
			//{
			//	m_offsetSize = OffsetSize(0, static_cast<uint32_t>(m_uniformBlock->GetSize()));
			//}

			m_uniformBlock = uniformBlock;
			m_data = malloc(static_cast<size_t>(m_uniformBlock->GetSize()));
			return false;
		}

		return true;
	}

	void PushHandler::BindPush(const CommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		vkCmdPushConstants(commandBuffer.GetCommandBuffer(), pipeline.GetPipelineLayout(), m_uniformBlock->GetStageFlags(),
			0, static_cast<uint32_t>(m_uniformBlock->GetSize()), m_data); // TODO: Allow offsets and sizes: m_offsetSize.GetOffset(), m_offsetSize.GetSize()
	}
}
