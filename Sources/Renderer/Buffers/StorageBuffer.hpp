#pragma once

#include <vulkan/vulkan.h>
#include "Renderer/Descriptors/Descriptor.hpp"
#include "Renderer/Pipelines/Shader.hpp"
#include "Buffer.hpp"

namespace acid
{
	class ACID_EXPORT StorageBuffer :
		public Descriptor,
		public Buffer
	{
	public:
		explicit StorageBuffer(const VkDeviceSize &size);

		void Update(const void *newData);

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage, const uint32_t &count);

		WriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType,
		                                      const VkDescriptorSet &descriptorSet, const std::optional<OffsetSize> &offsetSize) const override;
	};
}
