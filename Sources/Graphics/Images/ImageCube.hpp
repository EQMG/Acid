#pragma once

#include "Resources/Resource.hpp"
#include "Image.hpp"

namespace acid {
class Bitmap;

/**
 * @brief Resource that represents a cubemap image.
 */
class ACID_EXPORT ImageCube : public Image, public Resource {
public:
	/**
	 * Creates a new cubemap image, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The cubemap image with the requested values.
	 */
	static std::shared_ptr<ImageCube> Create(const Node &node);

	/**
	 * Creates a new cubemap image.
	 * @param filename The file to load the image from.
	 * @param fileSuffix The files extension type (ex .png).
	 * @param filter The magnification/minification filter to apply to lookups.
	 * @param addressMode The addressing mode for outside [0..1] range.
	 * @param anisotropic If anisotropic filtering is enabled.
	 * @param mipmap If mapmaps will be generated.
	 * @return The cubemap image with the requested values.
	 */
	static std::shared_ptr<ImageCube> Create(const std::filesystem::path &filename, const std::string &fileSuffix, VkFilter filter = VK_FILTER_LINEAR,
		VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, bool anisotropic = true, bool mipmap = true);

	/**
	 * Creates a new cubemap image.
	 * @param filename The file to load the image from.
	 * @param fileSuffix The files extension type (ex .png).
	 * @param filter The magnification/minification filter to apply to lookups.
	 * @param addressMode The addressing mode for outside [0..1] range.
	 * @param anisotropic If anisotropic filtering is enabled.
	 * @param mipmap If mapmaps will be generated.
	 * @param load If this resource will be loaded immediately, otherwise {@link ImageCube#Load} can be called later.
	 */
	explicit ImageCube(std::filesystem::path filename, std::string fileSuffix = ".png", VkFilter filter = VK_FILTER_LINEAR,
		VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, bool anisotropic = true, bool mipmap = true, bool load = true);

	/**
	 * Creates a new cubemap image.
	 * @param extent The images extent in pixels.
	 * @param format The format and type of the texel blocks that will be contained in the image.
	 * @param layout The layout that the image subresources accessible from.
	 * @param usage The intended usage of the image.
	 * @param filter The magnification/minification filter to apply to lookups.
	 * @param addressMode The addressing mode for outside [0..1] range.
	 * @param samples The number of samples per texel.
	 * @param anisotropic If anisotropic filtering is enabled.
	 * @param mipmap If mapmaps will be generated.
	 */
	ImageCube(const Vector2ui &extent, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
		VkFilter filter = VK_FILTER_LINEAR, VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT, bool anisotropic = false, bool mipmap = false);

	/**
	 * Creates a new cubemap image.
	 * @param bitmap The bitmap to load from.
	 * @param format The format and type of the texel blocks that will be contained in the image.
	 * @param layout The layout that the image subresources accessible from.
	 * @param usage The intended usage of the image.
	 * @param filter The magnification/minification filter to apply to lookups.
	 * @param addressMode The addressing mode for outside [0..1] range.
	 * @param samples The number of samples per texel.
	 * @param anisotropic If anisotropic filtering is enabled.
	 * @param mipmap If mapmaps will be generated.
	 */
	ImageCube(std::unique_ptr<Bitmap> &&bitmap, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
		VkFilter filter = VK_FILTER_LINEAR, VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT, bool anisotropic = false, bool mipmap = false);

	~ImageCube();

	/**
	 * Copies the images pixels from memory to a bitmap. The bitmap height will be scaled by the amount of layers.
	 * @param mipLevel The mipmap level index to sample.
	 * @return A copy of the images pixels.
	 */
	std::unique_ptr<Bitmap> GetBitmap(uint32_t mipLevel = 0) const;

	/**
	 * Sets the pixels of this image.
	 * @param pixels The pixels to copy from.
	 * @param layerCount The amount of layers contained in the pixels.
	 * @param baseArrayLayer The first layer to copy into.
	 */
	void SetPixels(const uint8_t *pixels, uint32_t layerCount, uint32_t baseArrayLayer);

	std::type_index GetTypeIndex() const override { return typeid(ImageCube); }

	const std::filesystem::path &GetFilename() const { return m_filename; }
	const std::string &GetFileSuffix() const { return m_fileSuffix; }
	const std::vector<std::string> &GetFileSides() const { return m_fileSides; }
	bool IsAnisotropic() const { return m_anisotropic; }
	bool IsMipmap() const { return m_mipmap; }
	uint32_t GetComponents() const { return m_components; }

private:
	friend const Node &operator>>(const Node &node, ImageCube &image);
	friend Node &operator<<(Node &node, const ImageCube &image);

	void Load(std::unique_ptr<Bitmap> loadBitmap = nullptr);

	std::filesystem::path m_filename;
	std::string m_fileSuffix;
	/// X, -X, +Y, -Y, +Z, -Z
	std::vector<std::string> m_fileSides = {"Right", "Left", "Top", "Bottom", "Back", "Front"};

	bool m_anisotropic;
	bool m_mipmap;
	uint32_t m_components = 0;
};
}
