#pragma once

#include "Texture.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a loaded cubemap texture.
	/// </summary>
	class Cubemap :
		public IResource
	{
	private:
		static const std::vector<std::string> SIDE_FILE_SUFFIXS;

		std::string m_filename;
		std::string m_fileExt;

		int32_t m_components;
		int32_t m_width, m_height, m_depth;
		VkDeviceSize m_imageSize;

		Buffer *m_buffer;
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
		Cubemap(const std::string &filename, const std::string &fileExt);

		/// <summary>
		/// Deconstructor for the cubemap object.
		/// </summary>
		~Cubemap();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorSet &descriptorSet) const;

		std::string GetFilename() override { return m_filename; };
	private:
		void CreateImage(const uint32_t &width, const uint32_t &height, const uint32_t &depth, const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, VkImage &image, VkDeviceMemory &imageMemory);

		void TransitionImageLayout(const VkImage &image, const VkImageLayout &oldLayout, const VkImageLayout &newLayout);

		void CopyBufferToImage(const uint32_t &width, const uint32_t &height, const uint32_t &depth, const VkBuffer &buffer, const VkImage &image);
	};
}
