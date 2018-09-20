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
		std::unique_ptr<StorageBuffer> m_storageBuffer;
		void *m_data; // TODO: Convert to unique_ptr
		bool m_changed;
	public:
		explicit StorageHandler(const bool &multipipeline = false);

		explicit StorageHandler(UniformBlock *uniformBlock, const bool &multipipeline = false);

		~StorageHandler();

		template<typename T>
		void Push(const T &object, const size_t &offset, const size_t &size)
		{
			memcpy((char *) m_data + offset, &object, size);
			m_changed = true;
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

		StorageBuffer *GetStorageBuffer() const { return m_storageBuffer.get(); }
	};
}
