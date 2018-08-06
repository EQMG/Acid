#pragma once

#include <string>
#include "DescriptorSet.hpp"

namespace acid
{
	class ACID_EXPORT IDescriptor
	{
	public:
		IDescriptor()
		{
		}

		virtual ~IDescriptor()
		{
		}

		virtual VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const = 0;
	};
}
