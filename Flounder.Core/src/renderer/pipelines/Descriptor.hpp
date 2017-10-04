#pragma once

#include "../../platforms/glfw/Platform.hpp"

namespace Flounder
{
	class Descriptor
	{
	public:
		Descriptor()
		{
		}

		virtual ~Descriptor()
		{
		}

		virtual VkDescriptorSetLayoutBinding GetDescriptorLayout(const uint32_t &binding) = 0;

		virtual VkDescriptorPoolSize GetDescriptorPool(const uint32_t &binding) = 0;

		virtual VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorSet &descriptorSet) = 0;
	};
}