#pragma once

#include "Graphics/Descriptors/Descriptor.hpp"
#include "Image.hpp"

namespace acid {
/**
 * @brief Resource that represents a depth stencil image.
 */
class ACID_EXPORT ImageDepth : public Descriptor {
public:
	explicit ImageDepth(const Vector2ui &extent, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

	~ImageDepth();

	WriteDescriptorSet GetWriteDescriptor(uint32_t binding, const VkDescriptorType &descriptorType, const std::optional<OffsetSize> &offsetSize) const override;

	static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage);

	const Vector2ui &GetExtent() const { return m_extent; }
	const VkImage &GetImage() const { return m_image; }
	const VkDeviceMemory &GetMemory() { return m_memory; }
	const VkSampler &GetSampler() const { return m_sampler; }
	const VkImageView &GetView() const { return m_view; }
	const VkFormat &GetFormat() const { return m_format; }

private:
	Vector2ui m_extent;

	VkImage m_image = VK_NULL_HANDLE;
	VkDeviceMemory m_memory = VK_NULL_HANDLE;
	VkSampler m_sampler = VK_NULL_HANDLE;
	VkImageView m_view = VK_NULL_HANDLE;
	VkFormat m_format;
};
}
