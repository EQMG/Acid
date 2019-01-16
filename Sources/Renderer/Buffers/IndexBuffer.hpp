#pragma once

#include "Buffer.hpp"

namespace acid
{
	class ACID_EXPORT IndexBuffer :
		public Buffer
	{
	public:
		IndexBuffer(const VkIndexType &indexType, const uint64_t &elementSize, const std::size_t &indexCount, const void *newData);

		const VkIndexType &GetIndexType() const { return m_indexType; }

		const uint32_t &GetIndexCount() const { return m_indexCount; }
	private:
		VkIndexType m_indexType;
		uint32_t m_indexCount;
	};
}
