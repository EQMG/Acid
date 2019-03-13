#pragma once

#include "Renderer/Buffers/Buffer.hpp"
#include "Renderer/Descriptors/Descriptor.hpp"

namespace acid
{
class ACID_EXPORT DepthStencil : public Descriptor, public Buffer
{
  public:
	DepthStencil(const uint32_t& width, const uint32_t& height, const VkSampleCountFlagBits& samples = VK_SAMPLE_COUNT_1_BIT);

	~DepthStencil();

	static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(const uint32_t& binding, const VkDescriptorType& descriptorType, const VkShaderStageFlags& stage);

	WriteDescriptorSet GetWriteDescriptor(const uint32_t& binding, const VkDescriptorType& descriptorType, const VkDescriptorSet& descriptorSet, const std::optional<OffsetSize>& offsetSize) const override;

	const uint32_t& GetWidth() const
	{
		return m_width;
	}

	const uint32_t& GetHeight() const
	{
		return m_height;
	}

	const VkImage& GetImage() const
	{
		return m_image;
	}

	const VkImageView& GetImageView() const
	{
		return m_imageView;
	}

	const VkFormat& GetFormat() const
	{
		return m_format;
	}

  private:
	uint32_t m_width, m_height;

	VkImage m_image;
	VkImageView m_imageView;
	VkSampler m_sampler;
	VkFormat m_format;
};
}
