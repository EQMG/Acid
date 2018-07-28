#pragma once

#include "Buffer.hpp"

namespace acid
{
	class ACID_EXPORT IndexBuffer :
		public Buffer
	{
	private:
		VkIndexType m_indexType;
		uint32_t m_indexCount;
	public:
		IndexBuffer(const VkIndexType &indexType, const uint64_t &elementSize, const size_t &indexCount, void *newData);

		~IndexBuffer();

		VkIndexType GetIndexType() const { return m_indexType; }

		uint32_t GetIndexCount() const { return m_indexCount; }
	};
}
