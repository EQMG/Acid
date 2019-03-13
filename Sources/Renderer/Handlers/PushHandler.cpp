#include "PushHandler.hpp"

namespace acid
{
PushHandler::PushHandler(const bool& multipipeline) : m_multipipeline(multipipeline), m_uniformBlock({}), m_data(nullptr) {}

PushHandler::PushHandler(const Shader::UniformBlock& uniformBlock, const bool& multipipeline) : m_multipipeline(multipipeline), m_uniformBlock(uniformBlock), m_data(std::make_unique<char[]>(m_uniformBlock->GetSize())) {}

bool PushHandler::Update(const std::optional<Shader::UniformBlock>& uniformBlock)
{
	if((m_multipipeline && !m_uniformBlock) || (!m_multipipeline && m_uniformBlock != uniformBlock))
		{
			m_uniformBlock = uniformBlock;
			m_data = std::make_unique<char[]>(m_uniformBlock->GetSize());
			return false;
		}

	return true;
}

void PushHandler::BindPush(const CommandBuffer& commandBuffer, const Pipeline& pipeline)
{
	vkCmdPushConstants(commandBuffer.GetCommandBuffer(), pipeline.GetPipelineLayout(), m_uniformBlock->GetStageFlags(), 0, static_cast<uint32_t>(m_uniformBlock->GetSize()), m_data.get());
}
}
