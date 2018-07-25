#include "Texture.hpp"

#include <cmath>
#include <cassert>
#include <Textures/stb_image.h>
#include "Renderer/Renderer.hpp"
#include "Helpers/FileSystem.hpp"

namespace acid
{
	static const std::string FALLBACK_PATH = "Undefined.png";
	static const float ANISOTROPY = 16.0f;

	Texture::Texture(const std::string &filename, const bool &hasAlpha, const bool &repeatEdges, const bool &mipmap, const bool &anisotropic, const bool &nearest, const uint32_t &numberOfRows) :
		IResource(),
		Buffer(LoadSize(filename), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		IDescriptor(),
		m_filename(filename),
		m_hasAlpha(hasAlpha),
		m_repeatEdges(repeatEdges),
		m_mipLevels(1),
		m_anisotropic(anisotropic),
		m_nearest(nearest),
		m_numberOfRows(numberOfRows),
		m_components(0),
		m_width(0),
		m_height(0),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM),
		m_imageInfo({})
	{
#if FL_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		if (!FileSystem::FileExists(filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", filename.c_str());
			m_filename = FALLBACK_PATH;
		}

		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		auto pixels = LoadPixels(m_filename, &m_width, &m_height, &m_components);

		m_mipLevels = mipmap ? GetMipLevels(m_width, m_height, 1) : 1;

		Buffer *bufferStaging = new Buffer(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice, bufferStaging->GetVkBufferMemory(), 0, m_size, 0, &data);
		memcpy(data, pixels, m_size);
		vkUnmapMemory(logicalDevice, bufferStaging->GetVkBufferMemory());

		CreateImage(m_width, m_height, 1, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image, m_bufferMemory);
		TransitionImageLayout(m_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);
		CopyBufferToImage(m_width, m_height, 1, bufferStaging->GetVkBuffer(), m_image);
		CreateMipmaps(m_image, m_width, m_height, 1, m_mipLevels);
		TransitionImageLayout(m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_mipLevels);
		CreateImageSampler(m_anisotropic, m_nearest, m_mipLevels, m_sampler);
		CreateImageView(m_image, VK_IMAGE_VIEW_TYPE_2D, m_format, m_mipLevels, m_imageView);

		Buffer::CopyBuffer(bufferStaging->GetVkBuffer(), m_buffer, m_size);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;

		delete bufferStaging;
		DeletePixels(pixels);
		m_filename = filename;

#if FL_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Texture '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	Texture::Texture(const uint32_t &width, const uint32_t &height, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage, float *pixels) :
		IResource(),
		Buffer(width * height * 4, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		IDescriptor(),
		m_filename(""),
		m_hasAlpha(false),
		m_repeatEdges(false),
		m_mipLevels(1),
		m_anisotropic(false),
		m_nearest(false),
		m_numberOfRows(1),
		m_components(4),
		m_width(width),
		m_height(height),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_sampler(VK_NULL_HANDLE),
		m_format(format),
		m_imageInfo({})
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		Buffer *bufferStaging = new Buffer(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (pixels == nullptr)
		{
			pixels = new float[width * height]();

			for (uint32_t i = 0; i < width * height; i++)
			{
				pixels[i] = 0.0f;
			}
		}

		void *data;
		vkMapMemory(logicalDevice, bufferStaging->GetVkBufferMemory(), 0, m_size, 0, &data);
		memcpy(data, pixels, m_size);
		vkUnmapMemory(logicalDevice, bufferStaging->GetVkBufferMemory());

		CreateImage(m_width, m_height, 1, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL, usage | VK_IMAGE_USAGE_SAMPLED_BIT |
			VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image, m_bufferMemory);
		TransitionImageLayout(m_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);
		CopyBufferToImage(m_width, m_height, 1, bufferStaging->GetVkBuffer(), m_image);
		TransitionImageLayout(m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);
		CreateImageSampler(m_anisotropic, m_nearest, m_mipLevels, m_sampler);
		CreateImageView(m_image, VK_IMAGE_VIEW_TYPE_2D, m_format, m_mipLevels, m_imageView);

		Buffer::CopyBuffer(bufferStaging->GetVkBuffer(), GetVkBuffer(), m_size);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;

		delete bufferStaging;
		delete[] pixels;
	}

	Texture::~Texture()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		vkDestroySampler(logicalDevice, m_sampler, allocator);
		vkDestroyImageView(logicalDevice, m_imageView, allocator);
		vkDestroyImage(logicalDevice, m_image, allocator);
	}

	DescriptorType Texture::CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage)
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

	VkWriteDescriptorSet Texture::GetVkWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const
	{
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet.GetVkDescriptorSet();
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &m_imageInfo;

		return descriptorWrite;
	}

	VkDeviceSize Texture::LoadSize(const std::string &filepath)
	{
		int width;
		int height;
		int components;

		if (!FileSystem::FileExists(filepath))
		{
			//	fprintf(stdout, "File does not exist: '%s'\n", filepath.c_str());

			if (stbi_info(FALLBACK_PATH.c_str(), &width, &height, &components) == 0)
			{
				assert(false && "Vulkan invalid texture file format.");
			}
		}
		else
		{
			if (stbi_info(filepath.c_str(), &width, &height, &components) == 0)
			{
				assert(false && "Vulkan invalid texture file format.");
			}
		}

		return width * height * 4;
	}

	VkDeviceSize Texture::LoadSize(const std::string &filename, const std::string &fileExt, const std::vector<std::string> &fileSuffixes)
	{
		VkDeviceSize size = 0;

		for (auto &suffix : fileSuffixes)
		{
			const std::string filepathSide = filename + "/" + suffix + fileExt;
			const VkDeviceSize sizeSide = LoadSize(filepathSide);
			size += sizeSide;
		}

		return size;
	}

	unsigned char *Texture::LoadPixels(const std::string &filepath, int *width, int *height, int *components)
	{
		if (!FileSystem::FileExists(filepath))
		{
			fprintf(stderr, "File does not exist: '%s'\n", filepath.c_str());
			return nullptr;
		}

		stbi_uc *data = nullptr;

		if (stbi_info(filepath.c_str(), width, height, components) == 0)
		{
			assert(false && "Vulkan invalid texture file format.");
		}

		data = stbi_load(filepath.c_str(), width, height, components, STBI_rgb_alpha);

		if (data == nullptr)
		{
			fprintf(stderr, "Unable to load texture: '%s'\n", filepath.c_str());
		}

		return data;
	}


	unsigned char *Texture::LoadPixels(const std::string &filename, const std::string &fileExt, const std::vector<std::string> &fileSuffixes, const size_t &bufferSize, int *width, int *height, int *depth, int *components)
	{
		stbi_uc *pixels = (stbi_uc *) malloc(bufferSize);
		stbi_uc *offset = pixels;

		for (auto &suffix : fileSuffixes)
		{
			std::string filepathSide = filename + "/" + suffix + fileExt;
			VkDeviceSize sizeSide = LoadSize(filepathSide);
			stbi_uc *pixelsSide = LoadPixels(filepathSide, width, height, components);
			depth = width;

			memcpy(offset, pixelsSide, sizeSide);
			offset += sizeSide;
			free(pixelsSide);
		}

		return pixels;
	}

	void Texture::DeletePixels(unsigned char *pixels)
	{
		stbi_image_free(pixels);
	}

	uint32_t Texture::GetMipLevels(const int32_t &width, const int32_t &height, const int32_t &depth)
	{
		return (uint32_t) std::floor(std::log2(std::max(width, std::max(height, depth)))) + 1;
	}

	void Texture::CreateImage(const int32_t &width, const int32_t &height, const int32_t &depth, const uint32_t &mipLevels, const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, VkImage &image, VkDeviceMemory &imageMemory, const uint32_t &arrayLayers)
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.extent.width = width;
		imageCreateInfo.extent.height = height;
		imageCreateInfo.extent.depth = depth;
		imageCreateInfo.mipLevels = mipLevels;
		imageCreateInfo.arrayLayers = arrayLayers;
		imageCreateInfo.format = format;
		imageCreateInfo.tiling = tiling;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage = usage;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		Display::ErrorVk(vkCreateImage(logicalDevice, &imageCreateInfo, allocator, &image));

		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(logicalDevice, image, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = Buffer::FindMemoryType(memoryRequirements.memoryTypeBits, properties);;

		Display::ErrorVk(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, allocator, &imageMemory));

		vkBindImageMemory(logicalDevice, image, imageMemory, 0);
	}

	void Texture::TransitionImageLayout(const VkImage &image, const VkImageLayout &oldLayout, const VkImageLayout &newLayout, const uint32_t &mipLevels, const uint32_t &layerCount)
	{
		CommandBuffer *commandBuffer = new CommandBuffer();

		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.oldLayout = oldLayout;
		imageMemoryBarrier.newLayout = newLayout;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
		imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
		imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
		imageMemoryBarrier.subresourceRange.layerCount = layerCount;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = 0;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
			newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else
		{
			throw std::invalid_argument("Unsupported texture layout transition!");
		}

		vkCmdPipelineBarrier(commandBuffer->GetVkCommandBuffer(), sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

		delete commandBuffer;
	}

	void Texture::CopyBufferToImage(const int32_t &width, const int32_t &height, const int32_t &depth, const VkBuffer &buffer, const VkImage &image, const uint32_t &layerCount)
	{
		CommandBuffer *commandBuffer = new CommandBuffer();

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = layerCount;
		region.imageOffset = {0, 0, 0};
		region.imageExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), static_cast<uint32_t>(depth)};

		vkCmdCopyBufferToImage(commandBuffer->GetVkCommandBuffer(), buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		delete commandBuffer;
	}

	void Texture::CreateMipmaps(const VkImage &image, const int32_t &width, const int32_t &height, const int32_t &depth, const uint32_t &mipLevels, const uint32_t &layerCount)
	{
		CommandBuffer *commandBuffer = new CommandBuffer();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layerCount;
		barrier.subresourceRange.levelCount = 1;

		for (uint32_t i = 1; i < mipLevels; i++)
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer->GetVkCommandBuffer(),
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			VkImageBlit blit = {};
			blit.srcOffsets[0] = {0, 0, 0};
			blit.srcOffsets[1] = {width >> (i - 1), height >> (i - 1), depth == 1 ? 1 : depth >> (i - 1)};
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = layerCount;
			blit.dstOffsets[0] = {0, 0, 0};
			blit.dstOffsets[1] = {width >> i, height >> i, depth == 1 ? 1 : depth >> i};
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = layerCount;

			vkCmdBlitImage(commandBuffer->GetVkCommandBuffer(),
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer->GetVkCommandBuffer(),
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer->GetVkCommandBuffer(),
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		delete commandBuffer;
	}

	void Texture::CreateImageSampler(const bool &anisotropic, const bool &nearest, const uint32_t &mipLevels, VkSampler &sampler)
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.magFilter = nearest ? VK_FILTER_NEAREST : VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = nearest ? VK_FILTER_NEAREST : VK_FILTER_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.anisotropyEnable = anisotropic ? VK_TRUE : VK_FALSE;
		samplerCreateInfo.maxAnisotropy = std::min(ANISOTROPY, Display::Get()->GetVkPhysicalDeviceProperties().limits.maxSamplerAnisotropy);
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
		samplerCreateInfo.compareEnable = VK_FALSE;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = static_cast<float>(mipLevels);

		Display::ErrorVk(vkCreateSampler(logicalDevice, &samplerCreateInfo, allocator, &sampler));
	}

	void Texture::CreateImageView(const VkImage &image, const VkImageViewType &type, const VkFormat &format, const uint32_t &mipLevels, VkImageView &imageView, const uint32_t &layerCount)
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = image;
		imageViewCreateInfo.viewType = type;
		imageViewCreateInfo.format = format;
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.subresourceRange = {};
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = mipLevels;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = layerCount;

		Display::ErrorVk(vkCreateImageView(logicalDevice, &imageViewCreateInfo, allocator, &imageView));
	}
}
