#include "DepthStencil.hpp"

#include "Display/Display.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
	static const std::vector<VkFormat> TRY_FORMATS =
	{
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D16_UNORM
	};

	DepthStencil::DepthStencil(const VkExtent3D &extent, const VkSampleCountFlagBits &samples) :
		IDescriptor(),
		m_image(VK_NULL_HANDLE),
		m_imageMemory(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_sampler(VK_NULL_HANDLE),
		m_format(VK_FORMAT_UNDEFINED),
		m_imageInfo({})
	{
		auto physicalDevice = Display::Get()->GetPhysicalDevice();

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

		Texture::CreateImage(extent.width, extent.height, extent.depth, VK_IMAGE_TYPE_2D, samples, 1, m_format, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_image, m_imageMemory, 1);
		Texture::CreateImageSampler(true, false, false, 1, m_sampler);
		Texture::CreateImageView(m_image, VK_IMAGE_VIEW_TYPE_2D, m_format, 1, m_imageView, 1);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;
	}

	DepthStencil::~DepthStencil()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroySampler(logicalDevice, m_sampler, nullptr);
		vkDestroyImageView(logicalDevice, m_imageView, nullptr);
		vkFreeMemory(logicalDevice, m_imageMemory, nullptr);
		vkDestroyImage(logicalDevice, m_image, nullptr);
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

	VkWriteDescriptorSet DepthStencil::GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const
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
}
