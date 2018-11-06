#pragma once

#include "Renderer/Descriptors/IDescriptor.hpp"
#include "Renderer/Buffers/Buffer.hpp"

namespace acid
{
	class ACID_EXPORT DepthStencil :
		public IDescriptor,
		public Buffer
	{
	private:
		uint32_t m_width, m_height;

		VkImage m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;

		VkDescriptorImageInfo m_imageInfo;
	public:
		DepthStencil(const uint32_t &width, const uint32_t &height, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		~DepthStencil();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const DescriptorSet &descriptorSet) const override;

		uint32_t GetWidth() const { return m_width; }

		uint32_t GetHeight() const { return m_height; }

		VkImage GetImage() const { return m_image; }

		VkImageView GetImageView() const { return m_imageView; }

		VkFormat GetFormat() const { return m_format; }
	};
}
