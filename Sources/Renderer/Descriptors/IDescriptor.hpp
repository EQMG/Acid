#pragma once

#include "DescriptorSet.hpp"

namespace acid
{
	class ACID_EXPORT IDescriptor
	{
	public:
		virtual VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const DescriptorSet &descriptorSet) const = 0;
	};
}
