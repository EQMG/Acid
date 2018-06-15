#pragma once

#include "Helpers/FormatString.hpp"
#include "Texture.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a loaded cubemap texture.
	/// </summary>
	class FL_EXPORT Cubemap :
		public IResource,
		public Buffer,
		public IDescriptor
	{
	private:
		static const std::vector<std::string> SIDE_FILE_SUFFIXES;

		std::string m_filename;
		std::string m_fileExt;

		uint32_t m_mipLevels;

		int32_t m_components;
		int32_t m_width, m_height, m_depth;

		VkImage m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;

		VkDescriptorImageInfo m_imageInfo;
	public:
		static std::shared_ptr<Cubemap> Resource(const std::string &filename, const std::string &fileExt)
		{
			std::string suffixToken = "/" + SIDE_FILE_SUFFIXES[0] + fileExt;
			std::string realFilename = Files::Get()->SearchFile(filename + suffixToken);
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
		Cubemap(const std::string &filename, const std::string &fileExt, const bool &mipmap = true);

		/// <summary>
		/// A new cubemap object from a array of pixels.
		/// </summary>
		Cubemap(const uint32_t &width, const uint32_t &height, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage, float *pixels = nullptr);

		/// <summary>
		/// Deconstructor for the cubemap object.
		/// </summary>
		~Cubemap();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetVkWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;

		std::string GetFilename() override { return m_filename; };

		std::string GetExtension() { return m_fileExt; };
	};
}
