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
		m_data(std::make_unique<char[]>(m_uniformBlock->GetSize()))
	{
	}

	bool PushHandler::Update(UniformBlock *uniformBlock)
	{
		if ((m_multipipeline && m_uniformBlock == nullptr) || (!m_multipipeline && m_uniformBlock != uniformBlock))
		{
			m_uniformBlock = uniformBlock;
			m_data = std::make_unique<char[]>(m_uniformBlock->GetSize());
			return false;
		}

		return true;
	}

	void PushHandler::BindPush(const CommandBuffer &commandBuffer, const IPipeline &pipeline)
	{
		vkCmdPushConstants(commandBuffer.GetCommandBuffer(), pipeline.GetPipelineLayout(), m_uniformBlock->GetStageFlags(),
			0, static_cast<uint32_t>(m_uniformBlock->GetSize()), m_data.get());
	}
}
