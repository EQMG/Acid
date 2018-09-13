#include "PushHandler.hpp"

namespace acid
{
	PushHandler::PushHandler(const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(nullptr),
		m_data(nullptr)
	{
	}

	PushHandler::PushHandler(UniformBlock *uniformBlock, const bool &multipipeline) :
		m_multipipeline(multipipeline),
		m_uniformBlock(uniformBlock),
		m_data(malloc(static_cast<size_t>(m_uniformBlock->GetSize())))
	{
	}

	PushHandler::~PushHandler()
	{
		free(m_data);
	}

	bool PushHandler::Update(UniformBlock *uniformBlock)
	{
		if ((m_multipipeline && m_uniformBlock == nullptr) || (!m_multipipeline && m_uniformBlock != uniformBlock))
		{
			free(m_data);

			m_uniformBlock = uniformBlock;
			m_data = malloc(static_cast<size_t>(m_uniformBlock->GetSize()));
			return false;
		}

		return true;
	}

	void PushHandler::BindPush(const CommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		vkCmdPushConstants(commandBuffer.GetCommandBuffer(), pipeline.GetPipelineLayout(), m_uniformBlock->GetStageFlags(),
			0, static_cast<uint32_t>(m_uniformBlock->GetSize()), m_data);
	}
}
