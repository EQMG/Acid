#include "Cubemap.hpp"

#include <cstring>
#include <cmath>
#include "../Devices/Display.hpp"

namespace Flounder
{
	const std::vector<std::string> Cubemap::SIDE_FILE_SUFFIXS = {"Right", "Left", "Top", "Bottom", "Back", "Front"};

	Cubemap::Cubemap(const std::string &filename, const std::string &fileExt, const bool &mipmap) :
		IResource(),
		Buffer(LoadSize(filename, fileExt), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		Descriptor(),
		m_filename(filename),
		m_fileExt(fileExt),
		m_components(0),
		m_width(0),
		m_height(0),
		m_depth(0),
		m_image(VK_NULL_HANDLE),
		m_imageMemory(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM),
		m_imageInfo({})
	{
#if FLOUNDER_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif

		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		stbi_uc *pixels = LoadPixels(filename, fileExt, static_cast<size_t>(Buffer::GetSize()), &m_width, &m_height, &m_depth, &m_components);

		m_mipLevels = 1; // mipmap ? (uint32_t)std::floor(std::log2(std::max(m_width, m_height))) + 1 : 1;

		Buffer *bufferStaging = new Buffer(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice, bufferStaging->GetBufferMemory(), 0, m_size, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(m_size));
		vkUnmapMemory(logicalDevice, bufferStaging->GetBufferMemory());

		m_imageMemory = GetBufferMemory();
		Texture::CreateImage(m_width, m_height, m_depth, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image, m_imageMemory, 6);
		Texture::TransitionImageLayout(m_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 6);
		Texture::CopyBufferToImage(static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height), static_cast<uint32_t>(m_depth), bufferStaging->GetBuffer(), m_image, 6);
		Texture::TransitionImageLayout(m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 6);

		// Generate the mip chain.
		for (uint32_t i = 1; i < m_mipLevels; i++)
		{
		//	stbi_uc *mipPixels = nullptr;
		//	stbir_resize_uint8(pixels, m_width, m_height, 0, mipPixels, m_width >> i, m_height >> i, 0, m_components);
		}

		// Create sampler.
		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.anisotropyEnable = VK_TRUE;
		samplerCreateInfo.maxAnisotropy = Display::Get()->GetPhysicalDeviceProperties().limits.maxSamplerAnisotropy;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
		samplerCreateInfo.compareEnable = VK_FALSE;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = (float)m_mipLevels;

		Platform::ErrorVk(vkCreateSampler(logicalDevice, &samplerCreateInfo, nullptr, &m_sampler));

		// Create image view.
		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = m_image;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		imageViewCreateInfo.format = m_format;
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.subresourceRange = {};
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = m_mipLevels;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 6;

		Platform::ErrorVk(vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &m_imageView));

		Buffer::CopyBuffer(bufferStaging->GetBuffer(), m_buffer, m_size);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;

		delete bufferStaging;
		free(pixels);

#if FLOUNDER_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();
		printf("Cubemap '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	Cubemap::~Cubemap()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroySampler(logicalDevice, m_sampler, nullptr);
		vkDestroyImage(logicalDevice, m_image, nullptr);
		vkDestroyImageView(logicalDevice, m_imageView, nullptr);
	}

	DescriptorType Cubemap::CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		descriptorSetLayoutBinding.stageFlags = stage;

		VkDescriptorPoolSize descriptorPoolSize = {};
		descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorPoolSize.descriptorCount = 1;

		return DescriptorType(binding, stage, descriptorSetLayoutBinding, descriptorPoolSize);
	}

	VkWriteDescriptorSet Cubemap::GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const
	{
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet.GetDescriptorSet();
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &m_imageInfo;

		return descriptorWrite;
	}
	
	VkDeviceSize Cubemap::LoadSize(const std::string &filename, const std::string &fileExt)
	{
		VkDeviceSize size = 0;
		
		for (const auto suffix : SIDE_FILE_SUFFIXS)
		{
			const std::string filepathSide = filename + "/" + suffix + fileExt;
			const VkDeviceSize sizeSide = Texture::LoadSize(filepathSide);
			size += sizeSide;
		}

		return size;
	}

	stbi_uc *Cubemap::LoadPixels(const std::string &filename, const std::string &fileExt, const size_t &bufferSize, int *width, int *height, int *depth, int *components)
	{
		stbi_uc *pixels = (stbi_uc *) malloc(bufferSize);
		stbi_uc *offset = pixels;

		for (const auto suffix : SIDE_FILE_SUFFIXS)
		{
			std::string filepathSide = filename + "/" + suffix + fileExt;
			VkDeviceSize sizeSide = Texture::LoadSize(filepathSide);
			stbi_uc *pixelsSide = Texture::LoadPixels(filepathSide, width, height, components);
			depth = width;

			memcpy(offset, pixelsSide, static_cast<size_t>(sizeSide));
			offset += sizeSide;
			free(pixelsSide);
		}

		return pixels;
	}
}
