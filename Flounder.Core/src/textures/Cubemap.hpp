#pragma once

#include "Texture.hpp"

namespace Flounder
{
	class Cubemap
	{
	private:
		static const std::vector<std::string> SIDE_FILE_SUFFIXS;

		int32_t m_components;
		int32_t m_width, m_height;
		VkDeviceSize m_imageSize;

		VkImage m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;
	public:
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
	};
}
