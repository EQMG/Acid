#pragma once

#include <string>
#include "Display/Display.hpp"
#include "DescriptorSet.hpp"

namespace Flounder
{
	class F_EXPORT IDescriptor
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
