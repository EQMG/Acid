#include "DepthStencil.hpp"
#include "../buffers/Buffer.hpp"

namespace Flounder
{
	DepthStencil::DepthStencil() :
		m_depthStencilImage(VK_NULL_HANDLE),
		m_depthStencilImageMemory(VK_NULL_HANDLE),
		m_depthStencilImageView(VK_NULL_HANDLE),
		m_depthStencilFormat(VK_FORMAT_UNDEFINED)
	{
	}

	DepthStencil::~DepthStencil()
	{
	}

	void DepthStencil::Create(const VkDevice &logicalDevice, const VkPhysicalDevice &physicalDevice, const VkPhysicalDeviceMemoryProperties &physicalDeviceMemoryProperties, const VkExtent3D &extent)
	{
		std::vector<VkFormat> tryFormats{
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D16_UNORM
		};

		for (auto format : tryFormats)
		{
			VkFormatProperties formatProperties = {};

			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

			if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				m_depthStencilFormat = format;
				break;
			}
		}

		if (m_depthStencilFormat != VK_FORMAT_D32_SFLOAT_S8_UINT &&
			m_depthStencilFormat != VK_FORMAT_D24_UNORM_S8_UINT &&
			m_depthStencilFormat != VK_FORMAT_D16_UNORM_S8_UINT &&
			m_depthStencilFormat != VK_FORMAT_S8_UINT)
		{
			m_depthStencilFormat = VK_FORMAT_UNDEFINED;
		}

		if (m_depthStencilFormat == VK_FORMAT_UNDEFINED)
		{
			assert(false && "Vulkan runtime error, depth stencil format not selected!");
		}

		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.flags = 0;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = m_depthStencilFormat;
		imageCreateInfo.extent = extent;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.queueFamilyIndexCount = VK_QUEUE_FAMILY_IGNORED;
		imageCreateInfo.pQueueFamilyIndices = nullptr;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		GlfwVulkan::ErrorVk(vkCreateImage(logicalDevice, &imageCreateInfo, nullptr, &m_depthStencilImage));

		VkMemoryRequirements imageMemoryRequirements = {};
		vkGetImageMemoryRequirements(logicalDevice, m_depthStencilImage, &imageMemoryRequirements);

		const uint32_t memoryTypeIndex = Buffer::FindMemoryType(physicalDevice, imageMemoryRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = imageMemoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

		GlfwVulkan::ErrorVk(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &m_depthStencilImageMemory));

		GlfwVulkan::ErrorVk(vkBindImageMemory(logicalDevice, m_depthStencilImage, m_depthStencilImageMemory, 0));

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = m_depthStencilImage;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = m_depthStencilFormat;
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | (
			m_depthStencilFormat != VK_FORMAT_UNDEFINED ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		GlfwVulkan::ErrorVk(vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &m_depthStencilImageView));
	}

	void DepthStencil::Cleanup(const VkDevice &device)
	{
		vkDestroyImageView(device, m_depthStencilImageView, nullptr);
		vkFreeMemory(device, m_depthStencilImageMemory, nullptr);
		vkDestroyImage(device, m_depthStencilImage, nullptr);
	}
}
