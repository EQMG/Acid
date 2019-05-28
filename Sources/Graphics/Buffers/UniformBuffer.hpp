#pragma once

#include <vulkan/vulkan.h>
#include "Graphics/Descriptors/Descriptor.hpp"
#include "Buffer.hpp"

namespace acid
{
class UniformBuffer :
	public Descriptor,
	public Buffer
{
public:
	explicit UniformBuffer(const VkDeviceSize &size, const void *data = nullptr);

	void Update(const void *newData);

	static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage,
		const uint32_t &count);

	WriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const std::optional<OffsetSize> &offsetSize) const override;
};
}
