#pragma once

#include "../../platforms/glfw/Platform.hpp"

namespace Flounder
{
	class Descriptior
	{
	public:
		Descriptior()
		{
		}

		virtual ~Descriptior()
		{
		}

		virtual VkDescriptorSetLayoutBinding GetDescriptorLayout() = 0;

		virtual VkDescriptorPoolSize GetDescriptorPool() = 0;

		virtual VkWriteDescriptorSet GetWriteDescriptor() = 0;
	};
}