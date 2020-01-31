#pragma once

#include "Image.hpp"

namespace acid {
/**
 * @brief Resource that represents an array of 2D images.
 */
class Image2dArray : public Image {
public:
	/**
	 * Creates a new array of 2D images.
	 * @param extent The images extent in pixels.
	 * @param arrayLayers The number of layers in the image .
	 * @param format The format and type of the texel blocks that will be contained in the image.
	 * @param layout The layout that the image subresources accessible from.
	 * @param usage The intended usage of the image.
	 * @param filter The magnification/minification filter to apply to lookups.
	 * @param addressMode The addressing mode for outside [0..1] range.
	 * @param anisotropic If anisotropic filtering is enabled.
	 * @param mipmap If mapmaps will be generated.
	 */
	Image2dArray(const Vector2ui &extent, uint32_t arrayLayers, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
		VkFilter filter = VK_FILTER_LINEAR, VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 
		bool anisotropic = false, bool mipmap = false);

	/**
	 * Creates a new array of 2D images.
	 * @param bitmap The bitmap to load from.
	 * @param arrayLayers The number of layers in the image .
	 * @param format The format and type of the texel blocks that will be contained in the image.
	 * @param layout The layout that the image subresources accessible from.
	 * @param usage The intended usage of the image.
	 * @param filter The magnification/minification filter to apply to lookups.
	 * @param addressMode The addressing mode for outside [0..1] range.
	 * @param anisotropic If anisotropic filtering is enabled.
	 * @param mipmap If mapmaps will be generated.
	 */
	Image2dArray(std::unique_ptr<Bitmap> &&bitmap, uint32_t arrayLayers, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
		VkFilter filter = VK_FILTER_LINEAR, VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		bool anisotropic = false, bool mipmap = false);

	/**
	 * Sets the pixels of this image.
	 * @param pixels The pixels to copy from.
	 * @param arrayLayer The layer to copy into.
	 */
	void SetPixels(const float *pixels, uint32_t arrayLayer);

	bool IsAnisotropic() const { return m_anisotropic; }
	bool IsMipmap() const { return m_mipmap; }
	
private:
	bool m_anisotropic;
	bool m_mipmap;
};
}
