#include "DepthStencil.hpp"

#include "Renderer/Buffers/Buffer.hpp"

namespace acid
{
	std::vector<VkFormat> TRY_FORMATS =
		{
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D16_UNORM
		};

	DepthStencil::DepthStencil(const VkExtent3D &extent) :
		IDescriptor(),
		m_image(VK_NULL_HANDLE),
		m_imageMemory(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_sampler(VK_NULL_HANDLE),
		m_format(VK_FORMAT_UNDEFINED),
		m_imageInfo({})
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto physicalDevice = Display::Get()->GetVkPhysicalDevice();

		for (auto &format : TRY_FORMATS)
		{
			VkFormatProperties formatProperties = {};
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

			if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT &&
				formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
			{
				m_format = format;
				break;
			}
		}

		if (m_format != VK_FORMAT_D32_SFLOAT_S8_UINT &&
			m_format != VK_FORMAT_D24_UNORM_S8_UINT &&
			m_format != VK_FORMAT_D16_UNORM_S8_UINT &&
			m_format != VK_FORMAT_S8_UINT)
		{
			m_format = VK_FORMAT_UNDEFINED;
		}

		if (m_format == VK_FORMAT_UNDEFINED)
		{
			throw std::runtime_error("Vulkan runtime error, depth stencil format not selected!");
		}

		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.flags = 0;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = m_format;
		imageCreateInfo.extent = extent;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.queueFamilyIndexCount = VK_QUEUE_FAMILY_IGNORED;
		imageCreateInfo.pQueueFamilyIndices = nullptr;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		Display::ErrorVk(vkCreateImage(logicalDevice, &imageCreateInfo, allocator, &m_image));

		VkMemoryRequirements imageMemoryRequirements = {};
		vkGetImageMemoryRequirements(logicalDevice, m_image, &imageMemoryRequirements);

		const uint32_t memoryTypeIndex = Buffer::FindMemoryType(imageMemoryRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = imageMemoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

		Display::ErrorVk(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, allocator, &m_imageMemory));

		Display::ErrorVk(vkBindImageMemory(logicalDevice, m_image, m_imageMemory, 0));

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = m_image;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = m_format;
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.subresourceRange = {};
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		Display::ErrorVk(vkCreateImageView(logicalDevice, &imageViewCreateInfo, allocator, &m_imageView));

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = 1.0f;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
		samplerCreateInfo.compareEnable = VK_FALSE;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		Display::ErrorVk(vkCreateSampler(logicalDevice, &samplerCreateInfo, allocator, &m_sampler));

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;
	}

	DepthStencil::~DepthStencil()
	{
		auto allocator = Display::Get()->GetVkAllocator();
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		vkDestroySampler(logicalDevice, m_sampler, allocator);
		vkDestroyImageView(logicalDevice, m_imageView, allocator);
		vkFreeMemory(logicalDevice, m_imageMemory, allocator);
		vkDestroyImage(logicalDevice, m_image, allocator);
	}

	DescriptorType DepthStencil::CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage)
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

	VkWriteDescriptorSet DepthStencil::GetVkWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const
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
}
