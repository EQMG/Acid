#pragma once

#include "Buffer.hpp"
#include "Renderer/Descriptors/Descriptor.hpp"
#include <vulkan/vulkan.h>

namespace acid
{
class ACID_EXPORT StorageBuffer : public Descriptor, public Buffer
{
  public:
	explicit StorageBuffer(const VkDeviceSize& size, const void* data = nullptr);

	void Update(const void* newData);

	static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(const uint32_t& binding, const VkDescriptorType& descriptorType, const VkShaderStageFlags& stage, const uint32_t& count);

	WriteDescriptorSet GetWriteDescriptor(const uint32_t& binding, const VkDescriptorType& descriptorType, const VkDescriptorSet& descriptorSet, const std::optional<OffsetSize>& offsetSize) const override;
};
}
