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
	public:
		explicit UniformHandler(const bool &multipipeline = false);

		explicit UniformHandler(const UniformBlock *uniformBlock, const bool &multipipeline = false);

		template<typename T>
		void Push(const T &object, const std::size_t &offset, const std::size_t &size)
		{
			if (memcmp(m_data.get() + offset, &object, size) != 0)
			{
				memcpy(m_data.get() + offset, &object, size);
				m_handlerStatus = Buffer::Status::Changed;
			}
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

		bool Update(const UniformBlock *uniformBlock);

		const UniformBuffer *GetUniformBuffer() const { return m_uniformBuffer.get(); }
	private:
		bool m_multipipeline;
		const UniformBlock *m_uniformBlock;
		uint32_t m_size;
		std::unique_ptr<char[]> m_data;
		std::unique_ptr<UniformBuffer> m_uniformBuffer;
		Buffer::Status m_handlerStatus;
	};
}
