#pragma once

#include <algorithm>
#include <memory>
#include "Renderer/Buffers/StorageBuffer.hpp"

namespace acid
{
	/// <summary>
	/// Class that handles a storage buffer.
	/// </summary>
	class ACID_EXPORT StorageHandler
	{
	private:
		bool m_multipipeline;
		UniformBlock *m_uniformBlock;
		uint32_t m_size;
		std::unique_ptr<char[]> m_data;
		std::unique_ptr<StorageBuffer> m_storageBuffer;
		HandlerStatus m_handlerStatus;
	public:
		explicit StorageHandler(const bool &multipipeline = false);

		explicit StorageHandler(UniformBlock *uniformBlock, const bool &multipipeline = false);

		void Push(void *data, const size_t &size)
		{
			if (size != m_size)
			{
				m_size = static_cast<uint32_t>(size);
				m_handlerStatus = HANDLER_STATUS_RESET;
				return;
			}

			if (m_uniformBlock == nullptr)
			{
				return;
			}

			if (memcmp(m_data.get(), data, size) != 0)
			{
				memcpy(m_data.get(), data, size);
				m_handlerStatus = HANDLER_STATUS_CHANGED;
			}
		}

		template<typename T>
		void Push(const T &object, const size_t &offset, const size_t &size)
		{
			if (m_uniformBlock == nullptr)
			{
				return;
			}

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

		StorageBuffer *GetStorageBuffer() const { return m_storageBuffer.get(); }
	};
}
