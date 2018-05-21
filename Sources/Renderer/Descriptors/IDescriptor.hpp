#pragma once

#include <string>
#include "Display/Display.hpp"
#include "DescriptorSet.hpp"

namespace fl
{
	class FL_EXPORT IDescriptor
	{
	public:
		IDescriptor()
		{
		}

		virtual ~IDescriptor()
		{
		}

		virtual VkWriteDescriptorSet GetVkWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const = 0;
	};
}
