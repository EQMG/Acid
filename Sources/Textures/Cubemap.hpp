#pragma once

#include "Resources/IResource.hpp"
#include "Texture.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a loaded cubemap texture.
	/// </summary>
	class ACID_EXPORT Cubemap :
		public IResource,
		public Buffer,
		public IDescriptor
	{
	private:
		static const std::vector<std::string> SIDE_FILE_SUFFIXES;

		std::string m_filename;
		std::string m_fileExt;

		bool m_repeatEdges;
		uint32_t m_mipLevels;
		bool m_anisotropic;
		bool m_nearest;

		uint32_t m_components;
		uint32_t m_width, m_height;

		VkImage m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;

		VkDescriptorImageInfo m_imageInfo;
	public:
		/// <summary>
		/// Will find an existing cubemap with the same filename, or create a new cubemap.
		/// </summary>
		/// <param name="filename"> The file base name (path without extension or face name). </param>
		/// <param name="fileExt"> The files extension type (ex .png). </param>
		static std::shared_ptr<Cubemap> Resource(const std::string &filename, const std::string &fileExt);

		/// <summary>
		/// A new cubemap object.
		/// </summary>
		/// <param name="filename"> The file base name (path without extension or face name). </param>
		/// <param name="fileExt"> The files extension type (ex .png). </param>
		/// <param name="repeatEdges"> If UV coords will wrap if outside of edge bounds. </param>
		/// <param name="mipmap"> If mipmaps will be used on the cubemap. </param>
		/// <param name="anisotropic"> If anisotropic will be use on the cubemap. </param>
		/// <param name="nearest"> If nearest filtering will be use on the cubemap. </param>
		Cubemap(const std::string &filename, const std::string &fileExt, const bool &repeatEdges = true, const bool &mipmap = true, const bool &anisotropic = true, const bool &nearest = false);

		/// <summary>
		/// A new empty cubemap object that can be used to render into.
		/// </summary>
		/// <param name="width"> The cubemaps width. </param>
		/// <param name="height"> The cubemaps height. </param>
		/// <param name="format"> The cubemaps format. </param>
		/// <param name="imageLayout"> The cubemaps image layout </param>
		/// <param name="usage"> The cubemaps image usage </param>
		/// <param name="samples"> The amount of MSAA samples to use. </param>
		Cubemap(const uint32_t &width, const uint32_t &height, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const VkImageLayout &imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		        const VkImageUsageFlags &usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		/// <summary>
		/// A new cubemap object from a array of pixels.
		/// </summary>
		/// <param name="width"> The cubemaps width. </param>
		/// <param name="height"> The cubemaps height. </param>
		/// <param name="pixels"> The inital pixels to use in the texture. <seealso cref="#GetPixels()"/> to get a copy of the pixels, and <seealso cref="#SetPixels()"/> to set the pixels</param>
		/// <param name="format"> The cubemaps format. </param>
		/// <param name="imageLayout"> The cubemaps image layout </param>
		/// <param name="usage"> The cubemaps image usage </param>
		/// <param name="samples"> The amount of MSAA samples to use. </param>
		Cubemap(const uint32_t &width, const uint32_t &height, float *pixels, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const VkImageLayout &imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				const VkImageUsageFlags &usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		~Cubemap();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;

		std::string GetFilename() override { return m_filename; };

		std::string GetExtension() { return m_fileExt; };

		uint32_t GetComponents() const { return m_components; }

		uint32_t GetWidth() const { return m_width; }

		uint32_t GetHeight() const { return m_height; }

		VkImage GetImage() const { return m_image; }

		VkImageView GetImageView() const { return m_imageView; }

		VkSampler GetSampler() const { return m_sampler; }
	};
}
