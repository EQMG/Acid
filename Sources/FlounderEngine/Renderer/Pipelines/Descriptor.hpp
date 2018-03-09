#pragma once

#include <string>
#include "../../Engine/Platform.hpp"
#include "DescriptorSet.hpp"

namespace Flounder
{
	class F_EXPORT Descriptor
	{
	public:
		Descriptor();

		virtual ~Descriptor();

	//	static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage, const VkDescriptorType &descriptorType);

		virtual VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const = 0;
	};
}
