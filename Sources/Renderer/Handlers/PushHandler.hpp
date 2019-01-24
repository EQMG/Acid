#pragma once

#include <algorithm>
#include <memory>
#include "Renderer/Descriptors/Descriptor.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
	/// <summary>
	/// Class that handles a push constant.
	/// </summary>
	class ACID_EXPORT PushHandler
	{
	public:
		explicit PushHandler(const bool &multipipeline = false);

		explicit PushHandler(const Shader::UniformBlock *uniformBlock, const bool &multipipeline = false);

		template<typename T>
		void Push(const T &object, const std::size_t &offset, const std::size_t &size)
		{
			memcpy(m_data.get() + offset, &object, size);
		}

		template<typename T>
		void Push(const std::string &uniformName, const T &object, const std::size_t &size = 0)
		{
			if (m_uniformBlock == nullptr)
			{
				return;
			}

			auto uniform = m_uniformBlock->GetUniform(uniformName);

			if (uniform == nullptr)
			{
				return;
			}

			std::size_t realSize = size;

			if (realSize == 0)
			{
				realSize = std::min(sizeof(object), static_cast<std::size_t>(uniform->GetSize()));
			}

			Push(object, static_cast<std::size_t>(uniform->GetOffset()), realSize);
		}

		bool Update(const Shader::UniformBlock *uniformBlock);

		void BindPush(const CommandBuffer &commandBuffer, const Pipeline &pipeline);
	private:
		bool m_multipipeline;
		const Shader::UniformBlock *m_uniformBlock;
		std::unique_ptr<char[]> m_data;
	};
}
