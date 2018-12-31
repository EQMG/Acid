#pragma once

#include "DescriptorSet.hpp"

namespace acid
{
	/// <summary>
	/// A wrapper for <seealso cref="VkWriteDescriptorSet"/> that has image and buffer image stored by reference.
	/// </summary>
	class WriteDescriptorSet :
		public VkWriteDescriptorSet
	{
	public:
		VkDescriptorImageInfo imageInfo;
		VkDescriptorBufferInfo bufferInfo;

		static VkWriteDescriptorSet GetType(const WriteDescriptorSet &writeDescriptorSet)
		{
			auto type = static_cast<VkWriteDescriptorSet>(writeDescriptorSet);
			type.pImageInfo = &writeDescriptorSet.imageInfo;
			type.pBufferInfo = &writeDescriptorSet.bufferInfo;
			return type;
		}

		static std::vector<VkWriteDescriptorSet> GetTypes(const std::vector<WriteDescriptorSet> &writeDescriptorSets)
		{
			std::vector<VkWriteDescriptorSet> types = {};

			for (auto &writeDescriptorSet : writeDescriptorSets)
			{
				types.emplace_back(GetType(writeDescriptorSet));
			}

			return types;
		}
	};

	class ACID_EXPORT OffsetSize
	{
	private:
		uint32_t m_offset;
		uint32_t m_size;
	public:
		OffsetSize(const uint32_t &offset, const uint32_t &size) :
			m_offset(offset),
			m_size(size)
		{
		}

		uint32_t GetOffset() const { return m_offset; }

		uint32_t GetSize() const { return m_size; }

		bool operator==(const OffsetSize &other) const
		{
			return m_offset == other.m_offset && m_size == other.m_size;
		}

		bool operator!=(const OffsetSize &other) const
		{
			return !(*this == other);
		}
	};

	class ACID_EXPORT Descriptor
	{
	public:
		virtual WriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType,
			const VkDescriptorSet &descriptorSet, const std::optional<OffsetSize> &offsetSize) const = 0;
	};
}
