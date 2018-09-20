#pragma once

#include <algorithm>
#include <memory>
#include "Renderer/Pipelines/Pipeline.hpp"

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
		void *m_data; // TODO: Convert to unique_ptr
	public:
		explicit PushHandler(const bool &multipipeline = false);

		explicit PushHandler(UniformBlock *uniformBlock, const bool &multipipeline = false);

		~PushHandler();

		template<typename T>
		void Push(const T &object, const size_t &offset, const size_t &size)
		{
			memcpy((char *) m_data + offset, &object, size);
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
#ifdef ACID_VERBOSE
			//	if (m_shaderProgram->ReportedNotFound(uniformName, true)) // TODO
			//	{
			//		Log::Error("Could not find uniform attribute in uniform '%s' of name '%s'\n", m_uniformBlock->GetName().c_str(), uniformName.c_str());
			//	}
#endif
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

		void BindPush(const CommandBuffer &commandBuffer, const Pipeline &pipeline);
	};
}
