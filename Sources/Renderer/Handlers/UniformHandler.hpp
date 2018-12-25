#pragma once

#include <algorithm>
#include <memory>
#include "Renderer/Buffers/UniformBuffer.hpp"

namespace acid
{
	/// <summary>
	/// Class that handles a uniform buffer.
	/// </summary>
	class ACID_EXPORT UniformHandler
	{
	private:
		bool m_multipipeline;
		UniformBlock *m_uniformBlock;
		uint32_t m_size;
		std::unique_ptr<char[]> m_data;
		std::unique_ptr<UniformBuffer> m_uniformBuffer;
		HandlerStatus m_handlerStatus;
	public:
		explicit UniformHandler(const bool &multipipeline = false);

		explicit UniformHandler(UniformBlock *uniformBlock, const bool &multipipeline = false);

		template<typename T>
		void Push(const T &object, const size_t &offset, const size_t &size)
		{
			if (memcmp(m_data.get() + offset, &object, size) != 0)
			{
				memcpy(m_data.get() + offset, &object, size);
				m_handlerStatus = HANDLER_STATUS_CHANGED;
			}
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

		UniformBuffer *GetUniformBuffer() const { return m_uniformBuffer.get(); }
	};
}
