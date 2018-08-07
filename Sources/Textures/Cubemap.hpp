#pragma once

#include "Helpers/FormatString.hpp"
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
		uint32_t m_width, m_height, m_depth;

		VkImage m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;

		VkDescriptorImageInfo m_imageInfo;
	public:
		static std::shared_ptr<Cubemap> Resource(const std::string &filename, const std::string &fileExt)
		{
			std::string suffixToken = "/" + SIDE_FILE_SUFFIXES[0] + fileExt;
			std::string realFilename = Files::SearchFile(filename + suffixToken);
			realFilename = FormatString::Replace(realFilename, suffixToken, "");
			auto resource = Resources::Get()->Get(realFilename);

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<Cubemap>(resource);
			}

			auto result = std::make_shared<Cubemap>(realFilename, fileExt);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		/// <summary>
		/// A new cubemap object.
		/// </summary>
		/// <param name="filename"> The file base name (path without extension or face name).. </param>
		/// <param name="fileExt"> The files extension type (ex .png). </param>
		/// <param name="repeatEdges"> If UV coords will wrap if outside of edge bounds. </param>
		/// <param name="mipmap"> If mipmaps will be used on the cubemap. </param>
		/// <param name="anisotropic"> If anisotropic will be use on the cubemap. </param>
		/// <param name="nearest"> If nearest filtering will be use on the cubemap. </param>
		Cubemap(const std::string &filename, const std::string &fileExt, const bool &repeatEdges = true, const bool &mipmap = true, const bool &anisotropic = true, const bool &nearest = false);

		/// <summary>
		/// A new cubemap object from a array of pixels.
		/// </summary>
		Cubemap(const uint32_t &width, const uint32_t &height, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage, float *pixels = nullptr);

		/// <summary>
		/// Deconstructor for the cubemap object.
		/// </summary>
		~Cubemap();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;

		std::string GetFilename() override { return m_filename; };

		std::string GetExtension() { return m_fileExt; };

		uint32_t GetComponents() const { return m_components; }

		uint32_t GetWidth() const { return m_width; }

		uint32_t GetHeight() const { return m_height; }

		uint32_t GetDepth() const { return m_depth; }

		VkImage GetImage() const { return m_image; }

		VkImageView GetImageView() const { return m_imageView; }

		VkSampler GetSampler() const { return m_sampler; }
	};
}
