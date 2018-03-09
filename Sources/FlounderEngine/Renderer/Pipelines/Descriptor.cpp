#include "Descriptor.hpp"

namespace Flounder
{
	Descriptor::Descriptor()
	{
	}

	Descriptor::~Descriptor()
	{
	}

	/*DescriptorType Descriptor::CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage, const VkDescriptorType &descriptorType)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		descriptorSetLayoutBinding.stageFlags = stage;

		VkDescriptorPoolSize descriptorPoolSize = {};
		descriptorPoolSize.type = descriptorType;
		descriptorPoolSize.descriptorCount = 1;

		return DescriptorType(binding, stage, descriptorSetLayoutBinding, descriptorPoolSize);
	}*/
}
