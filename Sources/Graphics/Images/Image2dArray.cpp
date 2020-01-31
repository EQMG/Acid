#include "Image2dArray.hpp"

#include "Bitmaps/Bitmap.hpp"
#include "Graphics/Buffers/Buffer.hpp"

namespace acid {
Image2dArray::Image2dArray(const Vector2ui &extent, uint32_t arrayLayers, VkFormat format, VkImageLayout layout, VkImageUsageFlags usage, VkFilter filter, VkSamplerAddressMode addressMode,
	bool anisotropic, bool mipmap) :
	Image(filter, addressMode, VK_SAMPLE_COUNT_1_BIT, layout,
		usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		format, 1, arrayLayers, {extent.m_x, extent.m_y, 1}),
	m_anisotropic(anisotropic),
	m_mipmap(mipmap) {
	if (m_extent.width == 0 || m_extent.height == 0) {
		return;
	}

	//m_mipLevels = m_mipmap ? GetMipLevels(m_extent) : 1;

	CreateImage(m_image, m_memory, m_extent, m_format, m_samples, VK_IMAGE_TILING_OPTIMAL,
		m_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_mipLevels, m_arrayLayers, VK_IMAGE_TYPE_2D);
	CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);
	CreateImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_2D_ARRAY, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, m_arrayLayers, 0);
	TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, m_arrayLayers, 0);
}

Image2dArray::Image2dArray(std::unique_ptr<Bitmap> &&bitmap, uint32_t arrayLayers, VkFormat format, VkImageLayout layout, VkImageUsageFlags usage, VkFilter filter,
	VkSamplerAddressMode addressMode, bool anisotropic, bool mipmap) :
	Image(filter, addressMode, VK_SAMPLE_COUNT_1_BIT, layout,
		usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		format, 1, arrayLayers, {bitmap->GetSize().m_x, bitmap->GetSize().m_y, 1}),
	m_anisotropic(anisotropic),
	m_mipmap(mipmap) {
	if (m_extent.width == 0 || m_extent.height == 0) {
		return;
	}

	//m_mipLevels = m_mipmap ? GetMipLevels(m_extent) : 1;

	CreateImage(m_image, m_memory, m_extent, m_format, m_samples, VK_IMAGE_TILING_OPTIMAL,
		m_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_mipLevels, m_arrayLayers, VK_IMAGE_TYPE_2D);
	CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);
	CreateImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_2D_ARRAY, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, m_arrayLayers, 0);
	TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, m_arrayLayers, 0);

	Buffer bufferStaging(bitmap->GetLength() * m_arrayLayers, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	uint8_t *data;
	bufferStaging.MapMemory(reinterpret_cast<void **>(&data));
	std::memcpy(data, bitmap->GetData().get(), bufferStaging.GetSize());
	bufferStaging.UnmapMemory();

	std::vector<VkBufferImageCopy> bufferCopyRegions;
	bufferCopyRegions.reserve(m_arrayLayers);
	for (uint32_t layer = 0; layer < m_arrayLayers; layer++) {
		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = layer;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = {0, 0, 0};
		region.imageExtent = m_extent;
		region.bufferOffset = 3 * m_extent.width * m_extent.height * layer;
		bufferCopyRegions.emplace_back(region);
	}
	CommandBuffer commandBuffer;
	vkCmdCopyBufferToImage(commandBuffer, bufferStaging.GetBuffer(), m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, bufferCopyRegions.size(), bufferCopyRegions.data());
	commandBuffer.SubmitIdle();
	
	TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, m_arrayLayers, 0);
}

void Image2dArray::SetPixels(const float *pixels, uint32_t arrayLayer) {
	Buffer bufferStaging(m_extent.width * m_extent.height * 3, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void *data;
	bufferStaging.MapMemory(&data);
	std::memcpy(data, pixels, bufferStaging.GetSize());
	bufferStaging.UnmapMemory();

	CopyBufferToImage(bufferStaging.GetBuffer(), m_image, m_extent, 1, arrayLayer);
}
}
