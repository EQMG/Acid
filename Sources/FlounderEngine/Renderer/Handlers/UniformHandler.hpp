#pragma once

#include "Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	class F_EXPORT UniformHandler
	{
	private:
		UniformBlock *m_uniformBlock;
		UniformBuffer *m_uniformBuffer;
		void *m_data;
		bool m_changed;
	public:
		UniformHandler();

		~UniformHandler();

		template<typename T>
		void Push(const T &object, const size_t &offset, const size_t &size)
		{
			memcpy(m_data + offset, &object, size);
			m_changed = true;
		}

		template<typename T>
		void Push(const std::string &uniformName, const T &object)
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

			Push(object, static_cast<size_t>(uniform->m_offset), static_cast<size_t>(uniform->m_size));
		}

		bool Update(UniformBlock *uniformBlock);

		UniformBuffer *GetUniformBuffer() const { return m_uniformBuffer; }
	};
}