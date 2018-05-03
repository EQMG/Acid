#pragma once

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
		static const std::vector<std::string> SIDE_FILE_SUFFIXS;

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
		static Cubemap *Resource(const std::string &filename, const std::string &fileExt)
		{
			IResource *resource = Resources::Get()->Get(filename);

			if (resource != nullptr)
			{
				return dynamic_cast<Cubemap *>(resource);
			}

			Cubemap *result = new Cubemap(filename, fileExt);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		/// <summary>
		/// A new cubemap object.
		/// </summary>
		Cubemap(const std::string &filename, const std::string &fileExt, const bool &mipmap = true);

		/// <summary>
		/// Deconstructor for the cubemap object.
		//		/// </summary>
		~Cubemap();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;

		std::string GetFilename() override { return m_filename; };

		static VkDeviceSize LoadSize(const std::string &filename, const std::string &fileExt);

		static stbi_uc *LoadPixels(const std::string &filename, const std::string &fileExt, const size_t &bufferSize, int *width, int *height, int *depth, int *components);
	};
}
