#pragma once

#include <vulkan/vulkan.h>
#include "Graphics/Descriptors/Descriptor.hpp"
#include "Buffer.hpp"

namespace acid {
class ACID_EXPORT StorageBuffer : public Descriptor, public Buffer {
public:
	explicit StorageBuffer(const VkDeviceSize &size, const void *data = nullptr);

	void Update(const void *newData);

	WriteDescriptorSet GetWriteDescriptor(uint32_t binding, const VkDescriptorType &descriptorType, const std::optional<OffsetSize> &offsetSize) const override;

	static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage,
		uint32_t count);
};
}
