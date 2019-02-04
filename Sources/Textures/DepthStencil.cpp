#include "DepthStencil.hpp"

#include <cassert>
#include "Renderer/Renderer.hpp"
#include "Texture.hpp"

namespace acid
{
	static const std::vector<VkFormat> TRY_FORMATS =
	{
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM
	};

	DepthStencil::DepthStencil(const uint32_t &width, const uint32_t &height, const VkSampleCountFlagBits &samples) :
		Descriptor(),
		Buffer(width * height * 4, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		m_width(width),
		m_height(height),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_sampler(VK_NULL_HANDLE),
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
			assert(false && "Vulkan runtime error, depth stencil format not selected!");
		}

		VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		if (Texture::HasStencil(m_format))
		{
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}

		Texture::CreateImage(m_image, m_bufferMemory, m_width, m_height, VK_IMAGE_TYPE_2D, samples, 1, m_format, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1);
		Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, aspectMask, 1, 0, 1);
		Texture::CreateImageSampler(m_sampler, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, false, 1);
		Texture::CreateImageView(m_image, m_imageView, VK_IMAGE_VIEW_TYPE_2D, m_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1, 0, 1);
	}

	DepthStencil::~DepthStencil()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		vkDestroySampler(logicalDevice->GetLogicalDevice(), m_sampler, nullptr);
		vkDestroyImageView(logicalDevice->GetLogicalDevice(), m_imageView, nullptr);
		vkDestroyImage(logicalDevice->GetLogicalDevice(), m_image, nullptr);
	}

	VkDescriptorSetLayoutBinding DepthStencil::GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = stage;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		return descriptorSetLayoutBinding;
	}

	WriteDescriptorSet DepthStencil::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType,
		const VkDescriptorSet &descriptorSet, const std::optional<OffsetSize> &offsetSize) const
	{
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.sampler = m_sampler;
		imageInfo.imageView = m_imageView;
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = descriptorType;
	//	descriptorWrite.pImageInfo = &imageInfo;
		return WriteDescriptorSet(descriptorWrite, imageInfo);
	}
}
