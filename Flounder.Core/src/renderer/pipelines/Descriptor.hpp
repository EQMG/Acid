#pragma once

#include "../../platforms/glfw/Platform.hpp"

namespace Flounder
{
	struct Descriptor
	{
		uint32_t bindingCount;
		const VkDescriptorSetLayoutBinding *pBindings;

		uint32_t poolSizeCount;
		VkDescriptorPoolSize *pPoolSizes;

	//	uint32_t descriptorWriteCount;
	//	VkWriteDescriptorSet *pDescriptorWrites;

		static Descriptor GenerateDescriptor(const std::vector<int> &descriptors)
		{
			std::vector<VkDescriptorSetLayoutBinding> bindings = std::vector<VkDescriptorSetLayoutBinding>();
			std::vector<VkDescriptorPoolSize> pools = std::vector<VkDescriptorPoolSize>();

			for (auto type : descriptors)
			{
				switch (type)
				{
				case 0:
					break;
				case 1:
					break;
				}
			}

			return { bindings.size(), bindings.data(), pools.size(), pools.data() };
		}
	};
}