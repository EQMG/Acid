#pragma once

#include <vector>

#include "Graphics/Commands/CommandBuffer.hpp"
#include "Graphics/Descriptors/Descriptor.hpp"

namespace acid {
class Bitmap;

/**
 * @brief A representation of a Vulkan image, sampler, and view.
 */
class ACID_EXPORT Image : public Descriptor {
public:
	/**
	 * Creates a new image object.
	 * @param extent The number of data elements in each dimension of the base level.
	 * @param imageType The dimensionality of the image.
	 * @param format The format and type of the texel blocks that will be contained in the image.
	 * @param samples The number of samples per texel.
	 * @param tiling The tiling arrangement of the texel blocks in memory.
	 * @param usage The intended usage of the image.
	 * @param properties The images memory properties.
	 * @param mipLevels The number of levels of detail available for minified sampling of the image.
	 * @param arrayLayers The number of layers in the image.
	 */
	Image(const VkExtent3D &extent, VkImageType imageType, VkFormat format, VkSampleCountFlagBits samples, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels, uint32_t arrayLayers);

	~Image();

	static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count);
	WriteDescriptorSet GetWriteDescriptor(uint32_t binding, VkDescriptorType descriptorType, const std::optional<OffsetSize> &offsetSize) const override;

	/**
	 * Copies the images pixels from memory to a bitmap. If this method is called from multiple threads at the same time Vulkan will crash!
	 * @param mipLevel The mipmap level index to sample.
	 * @param arrayLayer The array level to sample.
	 * @return A copy of the images pixels.
	 */
	std::unique_ptr<Bitmap> GetBitmap(uint32_t mipLevel = 0, uint32_t arrayLayer = 0) const;

	/**
	 * Sets the pixels of this image.
	 * @param pixels The pixels to copy from.
	 * @param layerCount The amount of layers contained in the pixels.
	 * @param baseArrayLayer The first layer to copy into.
	 */
	void SetPixels(const uint8_t *pixels, uint32_t layerCount, uint32_t baseArrayLayer);

	const VkExtent3D &GetExtent() const { return m_extent; }
	VkFormat GetFormat() const { return m_format; }
	VkSampleCountFlagBits GetSamples() const { return m_samples; }
	VkImageUsageFlags GetUsage() const { return m_usage; }
	uint32_t GetMipLevels() const { return m_mipLevels; }
	uint32_t GetArrayLevels() const { return m_arrayLayers; }
	VkFilter GetFilter() const { return m_filter; }
	VkSamplerAddressMode GetAddressMode() const { return m_addressMode; }
	bool IsAnisotropic() const { return m_anisotropic; }
	VkImageLayout GetLayout() const { return m_layout; }
	const VkImage &GetImage() { return m_image; }
	const VkDeviceMemory &GetMemory() { return m_memory; }
	const VkSampler &GetSampler() const { return m_sampler; }
	const VkImageView &GetView() const { return m_view; }

	static uint32_t GetMipLevels(const VkExtent3D &extent);

	/**
	 * Find a format in the candidates list that fits the tiling and features required.
	 * @param candidates Formats that are tested for features, in order of preference.
	 * @param tiling Tiling mode to test features in.
	 * @param features The features to test for.
	 * @return The format found, or VK_FORMAT_UNDEFINED.
	 */
	static VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	/**
	 * Gets if a format has a depth component.
	 * @param format The format to check.
	 * @return If the format has a depth component.
	 */
	static bool HasDepth(VkFormat format);

	/**
	 * Gets if a format has a depth component.
	 * @param format The format to check.
	 * @return If the format has a depth component.
	 */
	static bool HasStencil(VkFormat format);

	static void CreateImage(VkImage &image, VkDeviceMemory &memory, const VkExtent3D &extent, VkFormat format, VkSampleCountFlagBits samples,
		VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels, uint32_t arrayLayers, VkImageType type);
	static void CreateImageSampler(VkSampler &sampler, VkFilter filter, VkSamplerAddressMode addressMode, bool anisotropic, uint32_t mipLevels);
	static void CreateImageView(const VkImage &image, VkImageView &imageView, VkImageViewType type, VkFormat format, VkImageAspectFlags imageAspect,
		uint32_t mipLevels, uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer);
	static void CreateMipmaps(const VkImage &image, const VkExtent3D &extent, VkFormat format, VkImageLayout dstImageLayout, uint32_t mipLevels,
		uint32_t baseArrayLayer, uint32_t layerCount);
	static void TransitionImageLayout(const VkImage &image, VkFormat format, VkImageLayout srcImageLayout, VkImageLayout dstImageLayout,
		VkImageAspectFlags imageAspect, uint32_t mipLevels, uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer);
	static void InsertImageMemoryBarrier(const CommandBuffer &commandBuffer, const VkImage &image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
		VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
		VkImageAspectFlags imageAspect, uint32_t mipLevels, uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer);
	static void CopyBufferToImage(const VkBuffer &buffer, const VkImage &image, const VkExtent3D &extent, uint32_t layerCount, uint32_t baseArrayLayer);
	static bool CopyImage(const VkImage &srcImage, VkImage &dstImage, VkDeviceMemory &dstImageMemory, VkFormat srcFormat, const VkExtent3D &extent,
		VkImageLayout srcImageLayout, uint32_t mipLevel, uint32_t arrayLayer);

private:
	VkExtent3D m_extent;
	VkSampleCountFlagBits m_samples;
	VkImageUsageFlags m_usage;
	VkFormat m_format;
	uint32_t m_mipLevels;
	uint32_t m_arrayLayers;

	VkFilter m_filter;
	VkSamplerAddressMode m_addressMode;
	bool m_anisotropic;

	VkImageLayout m_layout;

	VkImage m_image = VK_NULL_HANDLE;
	VkDeviceMemory m_memory = VK_NULL_HANDLE;
	VkSampler m_sampler = VK_NULL_HANDLE;
	VkImageView m_view = VK_NULL_HANDLE;
};
}
