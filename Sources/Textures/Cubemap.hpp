#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Renderer/Descriptors/IDescriptor.hpp"
#include "Resources/IResource.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a loaded cubemap texture.
	/// </summary>
	class ACID_EXPORT Cubemap :
		public IResource,
		public IDescriptor
	{
	private:
		static const std::vector<std::string> FILE_SIDES;

		std::string m_filename;
		std::string m_fileSuffix;

		VkFilter m_filter;
		VkSamplerAddressMode m_addressMode;
		bool m_anisotropic;
		uint32_t m_mipLevels;
		VkSampleCountFlagBits m_samples;
		VkImageLayout m_imageLayout;

		uint32_t m_components;
		uint32_t m_width, m_height;

		VkImage m_image;
		VkDeviceMemory m_deviceMemory;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;

		VkDescriptorImageInfo m_imageInfo;
	public:
		/// <summary>
		/// Will find an existing cubemap with the same filename, or create a new cubemap.
		/// </summary>
		/// <param name="filename"> The file base name (path without extension or face name). </param>
		/// <param name="fileSuffix"> The files suffix type (ex .png). </param>
		static std::shared_ptr<Cubemap> Resource(const std::string &filename, const std::string &fileSuffix);

		/// <summary>
		/// A new cubemap object.
		/// </summary>
		/// <param name="filename"> The file base name (path without extension or face name). </param>
		/// <param name="fileExt"> The files extension type (ex .png). </param>
		/// <param name="filter"> The type of filtering will be use on the texture. </param>
		/// <param name="addressMode"> The sampler address mode to use. </param>
		/// <param name="anisotropic"> If anisotropic filtering will be use on the texture. </param>
		/// <param name="mipmap"> If mipmaps will be generated for the texture. </param>
		explicit Cubemap(const std::string &filename, const std::string &fileExt = ".png", const VkFilter &filter = VK_FILTER_LINEAR,
						 const VkSamplerAddressMode &addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT, const bool &anisotropic = true, const bool &mipmap = true);

		/// <summary>
		/// A new cubemap object from a array of pixels.
		/// </summary>
		/// <param name="width"> The cubemaps width. </param>
		/// <param name="height"> The cubemaps height. </param>
		/// <param name="pixels"> The inital pixels to use in the texture. <seealso cref="#GetPixels()"/> to get a copy of the pixels, and <seealso cref="#SetPixels()"/> to set the pixels. </param>
		/// <param name="format"> The cubemaps format. </param>
		/// <param name="imageLayout"> The cubemaps image layout </param>
		/// <param name="usage"> The cubemaps image usage </param>
		/// <param name="filter"> The type of filtering will be use on the cubemap. </param>
		/// <param name="addressMode"> The sampler address mode to use. </param>
		/// <param name="samples"> The amount of MSAA samples to use. </param>
		/// <param name="anisotropic"> If anisotropic filtering will be use on the texture. </param>
		/// <param name="mipmap"> If mipmaps will be generated for the texture. </param>
		Cubemap(const uint32_t &width, const uint32_t &height, void *pixels = nullptr, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const VkImageLayout &imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				const VkImageUsageFlags &usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, const VkFilter &filter = VK_FILTER_LINEAR, const VkSamplerAddressMode &addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
				const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT, const bool &anisotropic = false, const bool &mipmap = false);

		~Cubemap();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage, const uint32_t &count);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const DescriptorSet &descriptorSet) const override;

		/// <summary>
		/// Gets a copy of the face of a cubemaps pixels from memory, after usage is finished remember to delete the result.
		/// </summary>
		/// <param name="arrayLayer"> The layer to copy from. </param>
		/// <returns> A copy of the cubemaps pixels. </returns>
		uint8_t *GetPixels(const uint32_t &arrayLayer);

		/// <summary>
		/// Gets a copy of the cubemaps pixels from memory, after usage is finished remember to delete the result.
		/// </summary>
		/// <returns> A copy of the cubemaps. </returns>
		uint8_t *GetPixels();

		/// <summary>
		/// Copies the pixels into this cubemaps memory.
		/// </summary>
		/// <param name="pixels"> The pixels to copy to the image. </param>
		void SetPixels(uint8_t *pixels);

		std::string GetFilename() override { return m_filename; };

		std::string GetFileSuffix() { return m_fileSuffix; };

		VkFilter GetFilter() const { return m_filter; }

		VkSamplerAddressMode GetAddressMode() const { return m_addressMode; }

		bool IsAnisotropic() const { return m_anisotropic; }

		uint32_t GetMipLevels() const { return m_mipLevels; }

		VkSampleCountFlagBits GetSamples() const { return m_samples; }

		VkImageLayout GetImageLayout() const { return m_imageLayout; }

		uint32_t GetComponents() const { return m_components; }

		uint32_t GetWidth() const { return m_width; }

		uint32_t GetHeight() const { return m_height; }

		VkImage GetImage() const { return m_image; }

		VkImageView GetImageView() const { return m_imageView; }

		VkSampler GetSampler() const { return m_sampler; }
	};
}
