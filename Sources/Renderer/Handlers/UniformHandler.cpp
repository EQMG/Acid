#include "UniformHandler.hpp"

namespace acid
{
UniformHandler::UniformHandler(const bool& multipipeline) : m_multipipeline(multipipeline), m_uniformBlock({}), m_size(0), m_data(nullptr), m_uniformBuffer(nullptr), m_handlerStatus(Buffer::Status::Normal) {}

UniformHandler::UniformHandler(const Shader::UniformBlock& uniformBlock, const bool& multipipeline)
	: m_multipipeline(multipipeline), m_uniformBlock(uniformBlock), m_size(static_cast<uint32_t>(m_uniformBlock->GetSize())), m_data(std::make_unique<char[]>(m_size)), m_uniformBuffer(std::make_unique<UniformBuffer>(static_cast<VkDeviceSize>(m_size))),
	  m_handlerStatus(Buffer::Status::Normal)
{
}

bool UniformHandler::Update(const std::optional<Shader::UniformBlock>& uniformBlock)
{
	if(m_handlerStatus == Buffer::Status::Reset || (m_multipipeline && !m_uniformBlock) || (!m_multipipeline && m_uniformBlock != uniformBlock))
		{
			if((m_size == 0 && !m_uniformBlock) || (m_uniformBlock && m_uniformBlock != uniformBlock && static_cast<uint32_t>(m_uniformBlock->GetSize()) == m_size))
				{
					m_size = static_cast<uint32_t>(uniformBlock->GetSize());
				}

			m_uniformBlock = uniformBlock;
			m_data = std::make_unique<char[]>(m_size);
			m_uniformBuffer = std::make_unique<UniformBuffer>(static_cast<VkDeviceSize>(m_size));
			m_handlerStatus = Buffer::Status::Changed;
			return false;
		}

	if(m_handlerStatus != Buffer::Status::Normal)
		{
			m_uniformBuffer->Update(m_data.get());
			m_handlerStatus = Buffer::Status::Normal;
		}

	return true;
}
}
