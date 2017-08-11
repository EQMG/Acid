#include "texture.h"

namespace flounder
{
	texture::texture(std::string file, const bool &hasAlpha, const bool &clampEdges, const int32_t &mipLevels, const bool &anisotropic, const bool &nearest, const int32_t &numberOfRows)
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

		m_stagingBuffer = VK_NULL_HANDLE;
		m_stagingMemory = VK_NULL_HANDLE;

		m_image = VK_NULL_HANDLE;
		m_imageMemory = VK_NULL_HANDLE;
		m_imageView = VK_NULL_HANDLE;
		m_format = VK_FORMAT_UNDEFINED;
		m_sampler = VK_NULL_HANDLE;
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

		m_stagingBuffer = VK_NULL_HANDLE;
		m_stagingMemory = VK_NULL_HANDLE;

		m_image = VK_NULL_HANDLE;
		m_imageMemory = VK_NULL_HANDLE;
		m_imageView = VK_NULL_HANDLE;
		m_format = VK_FORMAT_UNDEFINED;
		m_sampler = VK_NULL_HANDLE;
		m_imageType = VK_IMAGE_TYPE_2D;
		m_components = 0;
		m_width = 0;
		m_height = 0;
		m_depth = 1;

		loadFromCubemap();
	}

	texture::~texture()
	{
		/*	vkDestroyBuffer(display::get()->getVkDevice(), m_stagingBuffer, nullptr);
			vkFreeMemory(display::get()->getVkDevice(), m_stagingMemory, nullptr);
	
			vkDestroyImage(display::get()->getVkDevice(), m_image, nullptr);
			vkFreeMemory(display::get()->getVkDevice(), m_imageMemory, nullptr);
			vkDestroyImageView(display::get()->getVkDevice(), m_imageView, nullptr);
			vkDestroySampler(display::get()->getVkDevice(), m_sampler, nullptr);*/
	}

	void texture::loadFromTexture()
	{
		/*if (stbi_info(file.c_str(), &m_width, &m_height, &m_components) == 0)
		{
			assert(0 && "Vulkan invalid texture file format.");
		}

		stbi_uc *stbidata = stbi_load(m_file.c_str(), &m_width, &m_height, &m_components, 0);

		if (stbidata == nullptr)
		{
			std::cout << "Unable to load texture: " << m_file << std::endl;
		}

		switch (m_components) {
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

		float *pixels = new float[m_width * m_height * m_components];
		
		for (int i = 0; i < m_width * m_height * m_components; i++)
		{
			float f = static_cast<float>(stbidata[i]) / static_cast<float>(static_cast<unsigned char>(-1));
			pixels[i] = f;
		}

		stbi_image_free(stbidata);

		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = m_width * m_height * m_components * sizeof(float);
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		display::vkErrorCheck(vkCreateBuffer(display::get()->getVkDevice(), &bufferCreateInfo, nullptr, &m_stagingBuffer));

		VkMemoryRequirements memoryRequirements = {};
		vkGetBufferMemoryRequirements(display::get()->getVkDevice(), m_stagingBuffer, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.pNext = NULL;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = 0;

		display::vkErrorCheck(vkAllocateMemory(display::get()->getVkDevice(), &memoryAllocateInfo, nullptr, &m_stagingMemory));

		display::vkErrorCheck(vkBindBufferMemory(display::get()->getVkDevice(), m_stagingBuffer, m_stagingMemory, 0));

		uint8_t *data;
		display::vkErrorCheck(vkMapMemory(display::get()->getVkDevice(), m_stagingMemory, 0, memoryRequirements.size, 0, (void**)&data));
		memcpy(data, pixels, bufferCreateInfo.size);
		vkUnmapMemory(display::get()->getVkDevice(), m_stagingMemory);

		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.flags = 0;
		imageCreateInfo.imageType = m_imageType;
		imageCreateInfo.format = m_format;
		imageCreateInfo.extent.width = m_width;
		imageCreateInfo.extent.height = m_height;
		imageCreateInfo.extent.depth = m_depth;
		imageCreateInfo.mipLevels = m_mipLevels;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;

		display::vkErrorCheck(vkCreateImage(display::get()->getVkDevice(), &imageCreateInfo, nullptr, &m_image));

		VkMemoryRequirements memReq = {};
		vkGetImageMemoryRequirements(display::get()->getVkDevice(), m_image, &memReq);

		uint32_t memoryType = 0;
		VkPhysicalDeviceMemoryProperties memProperties = {};
		vkGetPhysicalDeviceMemoryProperties(display::get()->getVkPhysicalDevice(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if ((memReq.memoryTypeBits & (1 << i)) && ((memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
			{
				memoryType = i;
			}
		}

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memReq.size;
		allocInfo.memoryTypeIndex = memoryType;

		display::vkErrorCheck(vkAllocateMemory(display::get()->getVkDevice(), &allocInfo, nullptr, &m_imageMemory));

		vkBindImageMemory(display::get()->getVkDevice(), m_image, m_imageMemory, 0);

		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = m_format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = m_mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };

		display::vkErrorCheck(vkCreateImageView(display::get()->getVkDevice(), &viewInfo, nullptr, &m_imageView));

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (display::get()->getVkProperties().limits.maxSamplerAnisotropy < 0.1f)
		{
			samplerInfo.anisotropyEnable = VK_FALSE;
		}
		else
		{
			samplerInfo.anisotropyEnable = VK_TRUE;
			samplerInfo.maxAnisotropy = 1.0f;
		//	samplerInfo.maxAnisotropy = display::get()->getVkProperties().limits.maxSamplerAnisotropy;
		}

		display::vkErrorCheck(vkCreateSampler(display::get()->getVkDevice(), &samplerInfo, nullptr, &m_sampler));

		delete[] pixels;*/
#if 0
		glGenTextures(1, &m_texture.m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_texture.m_textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texture.m_width, m_texture.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		if (m_mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			if (m_anisotropic)
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4); // TODO: FlounderTextures.get().getAnisotropyLevel()
			}
		}
		else if (m_nearest)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		if (m_clampEdges)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
#endif
	}

	void texture::loadFromCubemap()
	{
#if 0
		glGenTextures(1, &m_texture.m_textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture.m_textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (int i = 0; i < m_cubemapCount; i++)
		{
			int numComponents = 0;
			stbi_uc *data = stbi_load(m_cubemap[i].c_str(), &m_texture.m_width, &m_texture.m_height, &numComponents, 4);

			if (data == nullptr)
			{
				std::cout << "Unable to load texture: " << m_cubemap[i] << std::endl;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_texture.m_width, m_texture.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
#endif
	}
}
