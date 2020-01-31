#include "Image2dArray.hpp"

namespace acid {
Image2dArray::Image2dArray(const Vector2ui &extent, uint32_t layerCount, VkFormat format, VkImageLayout layout, VkImageUsageFlags usage, VkFilter filter, VkSamplerAddressMode addressMode,
	bool anisotropic, bool mipmap) :
	Image(filter, addressMode, VK_SAMPLE_COUNT_1_BIT, layout,
		usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		format, 1, 1, {extent.m_x, extent.m_y, 1}),
	m_anisotropic(anisotropic),
	m_mipmap(mipmap),
	layerCount(layerCount) {
	if (m_extent.width == 0 || m_extent.height == 0) {
		return;
	}

	m_mipLevels = m_mipmap ? GetMipLevels(m_extent) : 1;

	CreateImage(m_image, m_memory, m_extent, m_format, m_samples, VK_IMAGE_TILING_OPTIMAL,
		m_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_mipLevels, layerCount, VK_IMAGE_TYPE_2D);
	CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);
	CreateImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_2D_ARRAY, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, layerCount, 0);

	if (/*loadBitmap ||*/ m_mipmap) {
		TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, layerCount, 0);
	}

	if (m_mipmap) {
		CreateMipmaps(m_image, m_extent, m_format, m_layout, m_mipLevels, 0, layerCount);
	} /*else if (loadBitmap) {
		TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, layerCount, 0);
	}*/ else {
		TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, layerCount, 0);
	}
}
}
