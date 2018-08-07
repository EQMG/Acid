#include "Texture.hpp"

#include <cmath>
#include "Display/Display.hpp"
#include "Helpers/FileSystem.hpp"
#include "stb_image.h"
#include "stb_image_write.h"

namespace acid
{
	static const std::string FALLBACK_PATH = "Undefined.png";
	static const float ANISOTROPY = 16.0f;

	Texture::Texture(const std::string &filename, const bool &repeatEdges, const bool &mipmap, const bool &anisotropic, const bool &nearest) :
		IResource(),
		Buffer(LoadSize(filename), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		IDescriptor(),
		m_filename(filename),
		m_repeatEdges(repeatEdges),
		m_mipLevels(1),
		m_anisotropic(anisotropic),
		m_nearest(nearest),
		m_samples(),
		m_components(0),
		m_width(0),
		m_height(0),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM),
		m_imageInfo({})
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		if (!FileSystem::FileExists(filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", filename.c_str());
			m_filename = Files::SearchFile(FALLBACK_PATH);
		}

		auto logicalDevice = Display::Get()->GetLogicalDevice();

		auto pixels = LoadPixels(m_filename, &m_width, &m_height, &m_components);

		m_mipLevels = mipmap ? GetMipLevels(m_width, m_height, 1) : 1;

		Buffer bufferStaging = Buffer(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice, bufferStaging.GetBufferMemory(), 0, m_size, 0, &data);
		memcpy(data, pixels, m_size);
		vkUnmapMemory(logicalDevice, bufferStaging.GetBufferMemory());

		CreateImage(m_width, m_height, 1, VK_IMAGE_TYPE_2D, VK_SAMPLE_COUNT_1_BIT, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image, m_bufferMemory, 1);
		TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels, 1);
		CopyBufferToImage(m_width, m_height, 1, bufferStaging.GetBuffer(), m_image, 1);
		CreateMipmaps(m_image, m_width, m_height, 1, m_mipLevels, 1);
		TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_mipLevels, 1);
		CreateImageSampler(m_repeatEdges, m_anisotropic, m_nearest, m_mipLevels, m_sampler);
		CreateImageView(m_image, VK_IMAGE_VIEW_TYPE_2D, m_format, m_mipLevels, m_imageView, 1);

		Buffer::CopyBuffer(bufferStaging.GetBuffer(), m_buffer, m_size);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;

		DeletePixels(pixels);
		m_filename = filename;

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Texture '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	Texture::Texture(const int32_t &width, const int32_t &height, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage, const VkSampleCountFlagBits &samples, float *pixels) :
		IResource(),
		Buffer(width * height * 4, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		IDescriptor(),
		m_filename(""),
		m_repeatEdges(true),
		m_mipLevels(1),
		m_anisotropic(false),
		m_nearest(false),
		m_components(4),
		m_width(width),
		m_height(height),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_sampler(VK_NULL_HANDLE),
		m_format(format),
		m_imageInfo({})
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		Buffer *bufferStaging = new Buffer(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (pixels == nullptr)
		{
			pixels = new float[width * height]();

			for (int32_t i = 0; i < width * height; i++)
			{
				pixels[i] = 0.0f;
			}
		}

		void *data;
		vkMapMemory(logicalDevice, bufferStaging->GetBufferMemory(), 0, m_size, 0, &data);
		memcpy(data, pixels, m_size);
		vkUnmapMemory(logicalDevice, bufferStaging->GetBufferMemory());

		CreateImage(m_width, m_height, 1, VK_IMAGE_TYPE_2D, samples, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL, usage | VK_IMAGE_USAGE_SAMPLED_BIT |
			VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image, m_bufferMemory, 1);
		TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels, 1);
		CopyBufferToImage(m_width, m_height, 1, bufferStaging->GetBuffer(), m_image, 1);
	//	Texture::CreateMipmaps(m_image, m_width, m_height, 1, m_mipLevels, 1);
		TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_mipLevels, 1);
		CreateImageSampler(m_repeatEdges, m_anisotropic, m_nearest, m_mipLevels, m_sampler);
		CreateImageView(m_image, VK_IMAGE_VIEW_TYPE_2D, m_format, m_mipLevels, m_imageView, 1);

		Buffer::CopyBuffer(bufferStaging->GetBuffer(), GetBuffer(), m_size);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;

		delete bufferStaging;
		delete[] pixels;
	}

	Texture::~Texture()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroySampler(logicalDevice, m_sampler, nullptr);
		vkDestroyImageView(logicalDevice, m_imageView, nullptr);
		vkDestroyImage(logicalDevice, m_image, nullptr);
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

	VkWriteDescriptorSet Texture::GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const
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

	uint8_t *Texture::GetPixels()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkImage dstImage;
		VkDeviceMemory dstImageMemory;
		CopyImage(m_image, dstImage, dstImageMemory, m_width, m_height);

		VkImageSubresource imageSubresource = {};
		imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageSubresource.mipLevel = 0;
		imageSubresource.arrayLayer = 0;

		VkSubresourceLayout subresourceLayout;
		vkGetImageSubresourceLayout(logicalDevice, dstImage, &imageSubresource, &subresourceLayout);

		uint8_t *result = new uint8_t[subresourceLayout.size];

		void *data;
		vkMapMemory(logicalDevice, dstImageMemory, subresourceLayout.offset, subresourceLayout.size, 0, &data);
		memcpy(result, data, static_cast<size_t>(subresourceLayout.size));
		vkUnmapMemory(logicalDevice, dstImageMemory);

		vkFreeMemory(logicalDevice, dstImageMemory, nullptr);
		vkDestroyImage(logicalDevice, dstImage, nullptr);

		return result;
	}

	void Texture::SetPixels(uint8_t *pixels)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		Buffer *bufferStaging = new Buffer(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice, bufferStaging->GetBufferMemory(), 0, m_size, 0, &data);
		memcpy(data, pixels, m_size);
		vkUnmapMemory(logicalDevice, bufferStaging->GetBufferMemory());

		Buffer::CopyBuffer(bufferStaging->GetBuffer(), GetBuffer(), m_size);
	}

	int32_t Texture::LoadSize(const std::string &filepath)
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

	int32_t Texture::LoadSize(const std::string &filename, const std::string &fileExt, const std::vector<std::string> &fileSuffixes)
	{
		int32_t size = 0;

		for (auto &suffix : fileSuffixes)
		{
			std::string filepathSide = filename + "/" + suffix + fileExt;
			int32_t sizeSide = LoadSize(filepathSide);
			size += sizeSide;
		}

		return size;
	}

	uint8_t *Texture::LoadPixels(const std::string &filepath, int *width, int *height, int *components)
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

	uint8_t *Texture::LoadPixels(const std::string &filename, const std::string &fileExt, const std::vector<std::string> &fileSuffixes, const size_t &bufferSize, int *width, int *height, int *depth, int *components)
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

	bool Texture::WritePixels(const std::string &filename, const void *data, const int &width, const int &height, const int &components)
	{
		int result = stbi_write_png(filename.c_str(), width, height, components, data, width * components);
		return result == 1;
	}

	void Texture::DeletePixels(uint8_t *pixels)
	{
		stbi_image_free(pixels);
	}

	uint32_t Texture::GetMipLevels(const int32_t &width, const int32_t &height, const int32_t &depth)
	{
		return static_cast<uint32_t>(std::floor(std::log2(std::max(width, std::max(height, depth)))) + 1);
	}

	void Texture::CreateImage(const int32_t &width, const int32_t &height, const int32_t &depth, const VkImageType &type, const VkSampleCountFlagBits &samples, const uint32_t &mipLevels, const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, VkImage &image, VkDeviceMemory &imageMemory, const uint32_t &arrayLayers)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	//	imageCreateInfo.flags = arrayLayers == 6 ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
		imageCreateInfo.imageType = type;
		imageCreateInfo.extent.width = width;
		imageCreateInfo.extent.height = height;
		imageCreateInfo.extent.depth = depth;
		imageCreateInfo.mipLevels = mipLevels;
		imageCreateInfo.arrayLayers = arrayLayers;
		imageCreateInfo.format = format;
		imageCreateInfo.tiling = tiling;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage = usage;
		imageCreateInfo.samples = samples;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		Display::CheckVk(vkCreateImage(logicalDevice, &imageCreateInfo, nullptr, &image));

		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(logicalDevice, image, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = Buffer::FindMemoryType(memoryRequirements.memoryTypeBits, properties);;

		Display::CheckVk(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &imageMemory));

		vkBindImageMemory(logicalDevice, image, imageMemory, 0);
	}

	bool Texture::HasStencilComponent(const VkFormat &format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void Texture::TransitionImageLayout(const VkImage &image, const VkFormat &format, const VkImageLayout &oldLayout, const VkImageLayout &newLayout, const uint32_t &mipLevels, const uint32_t &layerCount)
	{
		CommandBuffer commandBuffer = CommandBuffer();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (HasStencilComponent(format))
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layerCount;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else
		{
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer.GetCommandBuffer(),
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);


		commandBuffer.End();
		commandBuffer.Submit();
	}

	void Texture::CopyBufferToImage(const int32_t &width, const int32_t &height, const int32_t &depth, const VkBuffer &buffer, const VkImage &image, const uint32_t &layerCount)
	{
		CommandBuffer commandBuffer = CommandBuffer();

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

		vkCmdCopyBufferToImage(commandBuffer.GetCommandBuffer(), buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		commandBuffer.End();
		commandBuffer.Submit();
	}

	void Texture::CreateMipmaps(const VkImage &image, const int32_t &width, const int32_t &height, const int32_t &depth, const uint32_t &mipLevels, const uint32_t &layerCount)
	{
		CommandBuffer commandBuffer = CommandBuffer();

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

			vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(),
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

			vkCmdBlitImage(commandBuffer.GetCommandBuffer(),
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(),
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

		vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(),
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		commandBuffer.End();
		commandBuffer.Submit();
	}

	void Texture::CreateImageSampler(const bool &repeatEdges, const bool &anisotropic, const bool &nearest, const uint32_t &mipLevels, VkSampler &sampler)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.magFilter = nearest ? VK_FILTER_NEAREST : VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = nearest ? VK_FILTER_NEAREST : VK_FILTER_LINEAR;
		samplerCreateInfo.addressModeU = repeatEdges ? VK_SAMPLER_ADDRESS_MODE_REPEAT : VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeV = repeatEdges ? VK_SAMPLER_ADDRESS_MODE_REPEAT : VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeW = repeatEdges ? VK_SAMPLER_ADDRESS_MODE_REPEAT : VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.anisotropyEnable = anisotropic ? VK_TRUE : VK_FALSE;
		samplerCreateInfo.maxAnisotropy = std::min(ANISOTROPY, Display::Get()->GetPhysicalDeviceProperties().limits.maxSamplerAnisotropy);
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
		samplerCreateInfo.compareEnable = VK_FALSE;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = static_cast<float>(mipLevels);

		Display::CheckVk(vkCreateSampler(logicalDevice, &samplerCreateInfo, nullptr, &sampler));
	}

	void Texture::CreateImageView(const VkImage &image, const VkImageViewType &type, const VkFormat &format, const uint32_t &mipLevels, VkImageView &imageView, const uint32_t &layerCount)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

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

		Display::CheckVk(vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &imageView));
	}

	bool Texture::CopyImage(const VkImage &srcImage, VkImage &dstImage, VkDeviceMemory &dstImageMemory, const uint32_t &width, const uint32_t &height)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto physicalDevice = Display::Get()->GetPhysicalDevice();
		auto surfaceFormat = Display::Get()->GetSurfaceFormat();
		auto physicalDeviceMemoryProperties = Display::Get()->GetPhysicalDeviceMemoryProperties();

		// Checks blit swapchain support.
		bool supportsBlit = true;
		VkFormatProperties formatProperties;

		// Check if the device supports blitting from optimal images (the swapchain images are in optimal format).
		vkGetPhysicalDeviceFormatProperties(physicalDevice, surfaceFormat.format, &formatProperties);

		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT))
		{
			supportsBlit = false;
		}

		// Check if the device supports blitting to linear images.
		vkGetPhysicalDeviceFormatProperties(physicalDevice, VK_FORMAT_R8G8B8A8_UNORM, &formatProperties);

		if (!(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT))
		{
			supportsBlit = false;
		}

		// Create the linear tiled destination image to copy to and to read the memory from.
		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		imageCreateInfo.extent.width = width;
		imageCreateInfo.extent.height = height;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
		imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		// Create the image.
		Display::CheckVk(vkCreateImage(logicalDevice, &imageCreateInfo, nullptr, &dstImage));

		// Create memory to back up the image.
		VkMemoryRequirements memoryRequirements;
		VkMemoryAllocateInfo memoryAllocateInfo{};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

		vkGetImageMemoryRequirements(logicalDevice, dstImage, &memoryRequirements);
		memoryAllocateInfo.allocationSize = memoryRequirements.size;

		// Memory must be host visible to copy from.
		memoryAllocateInfo.memoryTypeIndex = Display::Get()->FindMemoryTypeIndex(&physicalDeviceMemoryProperties, &memoryRequirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		Display::CheckVk(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &dstImageMemory));
		Display::CheckVk(vkBindImageMemory(logicalDevice, dstImage, dstImageMemory, 0));

		// Do the actual blit from the swapchain image to our host visible destination image.
		CommandBuffer commandBuffer = CommandBuffer();

		// Transition destination image to transfer destination layout.
		InsertImageMemoryBarrier(
			commandBuffer.GetCommandBuffer(),
			dstImage,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_GENERAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

		// Transition swapchain image from present to transfer source layout
		InsertImageMemoryBarrier(
			commandBuffer.GetCommandBuffer(),
			srcImage,
			VK_ACCESS_TRANSFER_READ_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

		// If source and destination support blit we'll blit as this also does automatic format conversion (e.g. from BGR to RGB).
		if (supportsBlit)
		{
			// Define the region to blit (we will blit the whole swapchain image).
			VkOffset3D blitSize;
			blitSize.x = width;
			blitSize.y = height;
			blitSize.z = 1;
			VkImageBlit imageBlitRegion{};
			imageBlitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlitRegion.srcSubresource.layerCount = 1;
			imageBlitRegion.srcOffsets[1] = blitSize;
			imageBlitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlitRegion.dstSubresource.layerCount = 1;
			imageBlitRegion.dstOffsets[1] = blitSize;

			// Issue the blit command
			vkCmdBlitImage(commandBuffer.GetCommandBuffer(), srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlitRegion, VK_FILTER_NEAREST);
		}
		else
		{
			// Otherwise use image copy (requires us to manually flip components).
			VkImageCopy imageCopyRegion{};
			imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.srcSubresource.layerCount = 1;
			imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.dstSubresource.layerCount = 1;
			imageCopyRegion.extent.width = static_cast<uint32_t>(width);
			imageCopyRegion.extent.height = static_cast<uint32_t>(height);
			imageCopyRegion.extent.depth = 1;

			// Issue the copy command.
			vkCmdCopyImage(commandBuffer.GetCommandBuffer(), srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
		}

		// Transition destination image to general layout, which is the required layout for mapping the image memory later on
		InsertImageMemoryBarrier(
			commandBuffer.GetCommandBuffer(),
			dstImage,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_GENERAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

		// Transition back the swap chain image after the blit is done
		InsertImageMemoryBarrier(
			commandBuffer.GetCommandBuffer(),
			srcImage,
			VK_ACCESS_TRANSFER_READ_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

		commandBuffer.End();
		commandBuffer.Submit();

		return supportsBlit;
	}

	void Texture::InsertImageMemoryBarrier(const VkCommandBuffer &cmdbuffer, const VkImage &image, const VkAccessFlags &srcAccessMask, const VkAccessFlags &dstAccessMask, const VkImageLayout &oldImageLayout, const VkImageLayout &newImageLayout, const VkPipelineStageFlags &srcStageMask, const VkPipelineStageFlags &dstStageMask, const VkImageSubresourceRange &subresourceRange)
	{
		VkImageMemoryBarrier imageMemoryBarrier{};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.srcAccessMask = srcAccessMask;
		imageMemoryBarrier.dstAccessMask = dstAccessMask;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		vkCmdPipelineBarrier(cmdbuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}
}
