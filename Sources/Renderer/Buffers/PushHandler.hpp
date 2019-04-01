#pragma once

#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
/**
 * @brief Class that handles a pipeline push constant.
 */
class ACID_EXPORT PushHandler
{
public:
	explicit PushHandler(const bool &multipipeline = false);

	explicit PushHandler(const Shader::UniformBlock &uniformBlock, const bool &multipipeline = false);

	template<typename T>
	void Push(const T &object, const std::size_t &offset, const std::size_t &size)
	{
		std::memcpy(m_data.get() + offset, &object, size);
	}

	template<typename T>
	void Push(const std::string &uniformName, const T &object, const std::size_t &size = 0)
	{
		if (!m_uniformBlock)
		{
			return;
		}

		auto uniform = m_uniformBlock->GetUniform(uniformName);

		if (!uniform)
		{
			return;
		}

		auto realSize = size;

		if (realSize == 0)
		{
			realSize = std::min(sizeof(object), static_cast<std::size_t>(uniform->GetSize()));
		}

		Push(object, static_cast<std::size_t>(uniform->GetOffset()), realSize);
	}

	bool Update(const std::optional<Shader::UniformBlock> &uniformBlock);

	void BindPush(const CommandBuffer &commandBuffer, const Pipeline &pipeline);

private:
	bool m_multipipeline;
	std::optional<Shader::UniformBlock> m_uniformBlock;
	std::unique_ptr<char[]> m_data;
};
}
