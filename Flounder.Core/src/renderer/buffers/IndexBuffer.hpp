#pragma once

#include "Buffer.hpp"

namespace Flounder
{
	class IndexBuffer :
		public Buffer
	{
	private:
		VkIndexType m_indexType;
		uint32_t m_indexCount;
	public:
		IndexBuffer();

		~IndexBuffer();

		void Create(const VkIndexType &indexType, const uint64_t &elementSize, const size_t &indexCount, void *newData);

		void Cleanup();

		VkIndexType GetIndexType() const { return m_indexType; }

		uint32_t GetIndexCount() const { return m_indexCount; }
	};
}
