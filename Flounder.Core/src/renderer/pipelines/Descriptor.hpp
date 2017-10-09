#pragma once

#include <vector>
#include "../../platforms/Platform.hpp"

namespace Flounder
{
	class DescriptorType
	{
	public:
		uint32_t binding;
		VkShaderStageFlags stage;

		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding;
		VkDescriptorPoolSize descriptorPoolSize;

		DescriptorType(const uint32_t &binding, const VkShaderStageFlags &stage, const VkDescriptorSetLayoutBinding &descriptorSetLayoutBinding, const VkDescriptorPoolSize &descriptorPoolSize) :
			binding(binding),
			stage(stage),
			descriptorSetLayoutBinding(descriptorSetLayoutBinding),
			descriptorPoolSize(descriptorPoolSize)
		{
		}
	};

	struct Descriptor
	{
		std::vector<VkDescriptorSetLayoutBinding> bindings;
		std::vector<VkDescriptorPoolSize> poolSizes;

		static Descriptor Create(const std::vector<DescriptorType> &types)
		{
			Descriptor descriptor = {};
			descriptor.bindings = std::vector<VkDescriptorSetLayoutBinding>();
			descriptor.poolSizes = std::vector<VkDescriptorPoolSize>();

			for (auto type : types)
			{
				descriptor.bindings.push_back(type.descriptorSetLayoutBinding);
				descriptor.poolSizes.push_back(type.descriptorPoolSize);
			}

			return descriptor;
		}
	};
}
