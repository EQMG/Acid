#include "texture.h"

namespace flounder
{
	texture::texture(std::string file, const bool &hasAlpha, const bool &clampEdges, const uint32_t &mipLevels, const bool &anisotropic, const bool &nearest, const uint32_t &numberOfRows)
	{
		m_file = file;
		m_cubemapCount = 0;
		m_cubemap = nullptr;

		m_hasAlpha = hasAlpha;
		m_clampEdges = clampEdges;
		m_mipLevels = mipLevels;
		m_anisotropic = anisotropic;
		m_nearest = nearest;
		m_numberOfRows = numberOfRows;

		m_image = VK_NULL_HANDLE;
		m_imageMemory = VK_NULL_HANDLE;
		m_imageView = VK_NULL_HANDLE;
		m_format = VK_FORMAT_UNDEFINED;
		m_imageType = VK_IMAGE_TYPE_2D;

		m_components = 0;
		m_width = 0;
		m_height = 0;
		m_depth = 1;

		loadFromTexture();
	}

	texture::texture(const int n_args, ...)
	{
		m_file = "";
		m_cubemapCount = n_args;
		m_cubemap = new std::string[n_args];

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_cubemap[i] = va_arg(ap, const char*);
		}

		va_end(ap);

		m_hasAlpha = false;
		m_clampEdges = false;
		m_mipLevels = 1;
		m_anisotropic = true;
		m_nearest = false;
		m_numberOfRows = 1;

		m_image = VK_NULL_HANDLE;
		m_imageMemory = VK_NULL_HANDLE;
		m_imageView = VK_NULL_HANDLE;
		m_format = VK_FORMAT_UNDEFINED;
		m_imageType = VK_IMAGE_TYPE_2D;

		m_components = 0;
		m_width = 0;
		m_height = 0;
		m_depth = 1;

		loadFromCubemap();
	}

	texture::~texture()
	{
		/*vkDestroyBuffer(display::get()->getVkDevice(), m_stagingBuffer, nullptr);
		vkFreeMemory(display::get()->getVkDevice(), m_stagingMemory, nullptr);
		vkDestroyImage(display::get()->getVkDevice(), m_image, nullptr);
		vkFreeMemory(display::get()->getVkDevice(), m_imageMemory, nullptr);
		vkDestroyImageView(display::get()->getVkDevice(), m_imageView, nullptr);

		m_stagingBuffer = VK_NULL_HANDLE;
		m_stagingMemory = VK_NULL_HANDLE;
		m_image = VK_NULL_HANDLE;
		m_imageMemory = VK_NULL_HANDLE;
		m_imageView = VK_NULL_HANDLE;*/
	}

	void texture::loadFromTexture()
	{
		float *pixels = loadPixels(m_file, &m_width, &m_height, &m_components);
		/*size_t componentSize = 4;

		switch (m_components)
		{
		case 1:
			m_format = VK_FORMAT_R32_SFLOAT;
			break;
		case 2:
			m_format = VK_FORMAT_R32G32_SFLOAT;
			break;
		case 3:
			m_format = VK_FORMAT_R32G32B32_SFLOAT;
			break;
		case 4:
			m_format = VK_FORMAT_R32G32B32A32_SFLOAT;
			break;
		default:
			assert(0 && "Vulkan texture components not between 1-4.");
		}

		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = m_width * m_height * m_components * componentSize;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		display::vkErrorCheck(vkCreateBuffer(display::get()->getVkDevice(), &bufferCreateInfo, nullptr, &m_stagingBuffer));

		VkMemoryRequirements memoryRequirements = {};
		vkGetBufferMemoryRequirements(display::get()->getVkDevice(), m_stagingBuffer, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.pNext = NULL;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = display::get()->memoryTypeIndex(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		display::vkErrorCheck(vkAllocateMemory(display::get()->getVkDevice(), &memoryAllocateInfo, nullptr, &m_stagingMemory));
		display::vkErrorCheck(vkBindBufferMemory(display::get()->getVkDevice(), m_stagingBuffer, m_stagingMemory, 0));

		uint8_t *data;
		display::vkErrorCheck(vkMapMemory(display::get()->getVkDevice(), m_stagingMemory, 0, memoryRequirements.size, 0, (void**) &data));
		memcpy(data, pixels, bufferCreateInfo.size);
		vkUnmapMemory(display::get()->getVkDevice(), m_stagingMemory);

		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.pNext = nullptr;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = m_format;
		imageCreateInfo.extent.width = m_width;
		imageCreateInfo.extent.height = m_height;
		imageCreateInfo.extent.depth = m_depth;
		imageCreateInfo.mipLevels = 1; // TODO
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;

		display::vkErrorCheck(vkCreateImage(display::get()->getVkDevice(), &imageCreateInfo, nullptr, &m_image));

		vkGetImageMemoryRequirements(display::get()->getVkDevice(), m_image, &memoryRequirements);

		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = display::get()->memoryTypeIndex(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		display::vkErrorCheck(vkAllocateMemory(display::get()->getVkDevice(), &memoryAllocateInfo, nullptr, &m_imageMemory));
		display::vkErrorCheck(vkBindImageMemory(display::get()->getVkDevice(), m_image, m_imageMemory, 0));

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1; // TODO
		subresourceRange.layerCount = 1;

		setImageLayout(
			renderer::get()->getVkCommandBuffers()[0],
			m_image,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			subresourceRange
		);

		VkBufferImageCopy bufferCopyRegion = {};
		bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferCopyRegion.imageSubresource.mipLevel = 0;
		bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
		bufferCopyRegion.imageSubresource.layerCount = 1;
		bufferCopyRegion.imageExtent.width = m_width;
		bufferCopyRegion.imageExtent.height = m_height;
		bufferCopyRegion.imageExtent.depth = m_depth;
		bufferCopyRegion.bufferOffset = 0;

		vkCmdCopyBufferToImage(
			renderer::get()->getVkCommandBuffers()[0],
			m_stagingBuffer,
			m_image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&bufferCopyRegion
		);

		setImageLayout(
			renderer::get()->getVkCommandBuffers()[0],
			m_image,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		);

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.pNext = NULL;
		imageViewCreateInfo.image = m_image;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = m_format;
		imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;
		imageViewCreateInfo.subresourceRange.levelCount = 1;

		display::vkErrorCheck(vkCreateImageView(display::get()->getVkDevice(), &imageViewCreateInfo, nullptr, &m_imageView));*/

		delete[] pixels;
	}

	void texture::loadFromCubemap()
	{
	}

	float *texture::loadPixels(const std::string &filepath, int *width, int *height, int *components)
	{
		stbi_uc *data = nullptr;
#ifdef FLOUNDER_PLATFORM_ANDROID
		AAsset* asset = AAssetManager_open((display::get()->getAssetManager(), filepath.c_str(), AASSET_MODE_STREAMING);

		if (!asset) 
		{
			std::cout << "Unable to load android asset: " << m_file << std::endl;
			freePixels(layersPixels);
			return nullptr;
		}

		uint32_t size = AAsset_getLength(asset);

		if (size <= 0) 
		{
			std::cout << "Android asset is empty: " << m_file << std::endl;
			freePixels(layersPixels);
			return nullptr;
		}

		unsigned char* androidData = new unsigned char[size];

		AAsset_read(asset, reinterpret_cast<char*>(androidData), size);
		AAsset_close(asset);

		data = stbi_load_from_memory(androidData, size, width, height, components, STBI_rgb_alpha);

		delete[] data;
#else
		if (stbi_info(filepath.c_str(), width, height, components) == 0)
		{
			assert(0 && "Vulkan invalid texture file format.");
		}

		data = stbi_load(filepath.c_str(), width, height, components, STBI_rgb_alpha);
#endif

		if (data == nullptr)
		{
			std::cout << "Unable to load texture: " << m_file << std::endl;
		}

		int pixelsSize = (*width) * (*height) * (*components);
		float *pixels = new float[pixelsSize];

		for (int i = 0; i < pixelsSize; i++)
		{
			float f = (float) data[i] / (float) (unsigned char) (-1);
			pixels[i] = f;
		}

		stbi_image_free(data);

		return pixels;
	}

	void texture::setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout)
	{
		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = aspectMask;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 1;

		setImageLayout(cmdbuffer, image, oldImageLayout, newImageLayout, subresourceRange);
	}

	void texture::setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange)
	{
		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.pNext = nullptr;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		switch (oldImageLayout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			imageMemoryBarrier.srcAccessMask = 0;
			break;
		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		}

		switch (newImageLayout)
		{
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = imageMemoryBarrier.srcAccessMask | VK_ACCESS_TRANSFER_READ_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			if (imageMemoryBarrier.srcAccessMask == 0)
			{
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			}

			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		}

		vkCmdPipelineBarrier(cmdbuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}
}
