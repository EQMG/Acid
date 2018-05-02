#pragma once

#include "Renderer/Descriptors/IDescriptor.hpp"

namespace fl
{
	class FL_EXPORT DepthStencil :
		public IDescriptor
	{
	private:
		VkImage m_image;
		VkDeviceMemory m_imageMemory;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;

		VkDescriptorImageInfo m_imageInfo;
	public:
		DepthStencil(const VkExtent3D &extent);

		~DepthStencil();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;

		VkImage GetImage() const { return m_image; }

		VkDeviceMemory GetImageMemory() const { return m_imageMemory; }

		VkImageView GetImageView() const { return m_imageView; }

		VkFormat GetFormat() const { return m_format; }
	};
}
