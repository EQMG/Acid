#pragma once

#include <algorithm>
#include <memory>
#include "Renderer/Descriptors/IDescriptor.hpp"
#include "Renderer/Pipelines/IPipeline.hpp"

namespace acid
{
	/// <summary>
	/// Class that handles a push constant.
	/// </summary>
	class ACID_EXPORT PushHandler
	{
	private:
		bool m_multipipeline;
		UniformBlock *m_uniformBlock;
		std::unique_ptr<char[]> m_data;
	public:
		explicit PushHandler(const bool &multipipeline = false);

		explicit PushHandler(UniformBlock *uniformBlock, const bool &multipipeline = false);

		template<typename T>
		void Push(const T &object, const size_t &offset, const size_t &size)
		{
			memcpy(m_data.get() + offset, &object, size);
		}

		template<typename T>
		void Push(const std::string &uniformName, const T &object, const size_t &size = 0)
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

			size_t realSize = size;

			if (realSize == 0)
			{
				realSize = std::min(sizeof(object), static_cast<size_t>(uniform->GetSize()));
			}

			Push(object, static_cast<size_t>(uniform->GetOffset()), realSize);
		}

		bool Update(UniformBlock *uniformBlock);

		void BindPush(const CommandBuffer &commandBuffer, const IPipeline &pipeline);
	};
}
