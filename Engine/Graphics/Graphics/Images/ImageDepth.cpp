#include "ImageDepth.hpp"

#include "Graphics/Graphics.hpp"

namespace acid {
static const std::vector<VkFormat> TRY_FORMATS = {
	VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT,
	VK_FORMAT_D16_UNORM
};

ImageDepth::ImageDepth(const Vector2ui &extent, VkSampleCountFlagBits samples) :
	Image(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, samples, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
		FindSupportedFormat(TRY_FORMATS, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT), 
		1, 1, {extent.x, extent.y, 1}) {
	if (format == VK_FORMAT_UNDEFINED)
		throw std::runtime_error("No depth stencil format could be selected");

	VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	if (HasStencil(format))
		aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

	CreateImage(image, memory, this->extent, format, samples, VK_IMAGE_TILING_OPTIMAL,
		usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1, 1, VK_IMAGE_TYPE_2D);
	CreateImageSampler(sampler, filter, addressMode, false, 1);
	CreateImageView(image, view, VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_DEPTH_BIT, 1, 0, 1, 0);
	TransitionImageLayout(image, format, VK_IMAGE_LAYOUT_UNDEFINED, layout, aspectMask, 1, 0, 1, 0);
}
}
