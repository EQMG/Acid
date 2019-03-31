#include "ImageDepth.hpp"

#include "Renderer/Buffers/Buffer.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
static const std::vector<VkFormat> TRY_FORMATS = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT,
	VK_FORMAT_D16_UNORM };

ImageDepth::ImageDepth(const uint32_t &width, const uint32_t &height, const VkSampleCountFlagBits &samples) :
	m_width(width),
	m_height(height),
	m_image(VK_NULL_HANDLE),
	m_memory(VK_NULL_HANDLE),
	m_sampler(VK_NULL_HANDLE),
	m_view(VK_NULL_HANDLE),
	m_format(VK_FORMAT_UNDEFINED)
{
	auto physicalDevice = Renderer::Get()->GetPhysicalDevice();

	for (const auto &format : TRY_FORMATS)
	{
		VkFormatProperties formatProperties = {};
		vkGetPhysicalDeviceFormatProperties(physicalDevice->GetPhysicalDevice(), format, &formatProperties);

		if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		{
			m_format = format;
			break;
		}
	}

	if (m_format == VK_FORMAT_UNDEFINED)
	{
		throw std::runtime_error("No depth stencil format could be selected");
	}

	VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

	if (Image::HasStencil(m_format))
	{
		aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}

	Image::CreateImage(m_image, m_memory, { m_width, m_height, 1 }, m_format, samples, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1, 1, VK_IMAGE_TYPE_2D);
	Image::CreateImageSampler(m_sampler, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, false, 1);
	Image::CreateImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_2D, m_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1, 0, 1, 0);
	Image::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, aspectMask, 1, 0, 1, 0);
}

ImageDepth::~ImageDepth()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	vkDestroyImageView(logicalDevice->GetLogicalDevice(), m_view, nullptr);
	vkDestroySampler(logicalDevice->GetLogicalDevice(), m_sampler, nullptr);
	vkFreeMemory(logicalDevice->GetLogicalDevice(), m_memory, nullptr);
	vkDestroyImage(logicalDevice->GetLogicalDevice(), m_image, nullptr);
}

VkDescriptorSetLayoutBinding ImageDepth::GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage)
{
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
	descriptorSetLayoutBinding.binding = binding;
	descriptorSetLayoutBinding.descriptorType = descriptorType;
	descriptorSetLayoutBinding.descriptorCount = 1;
	descriptorSetLayoutBinding.stageFlags = stage;
	descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
	return descriptorSetLayoutBinding;
}

WriteDescriptorSet ImageDepth::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const std::optional<OffsetSize> &offsetSize) const
{
	VkDescriptorImageInfo imageInfo = {};
	imageInfo.sampler = m_sampler;
	imageInfo.imageView = m_view;
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = VK_NULL_HANDLE; // Will be set in the descriptor handler.
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.descriptorType = descriptorType;
	//descriptorWrite.pImageInfo = &imageInfo;
	return WriteDescriptorSet(descriptorWrite, imageInfo);
}
}
