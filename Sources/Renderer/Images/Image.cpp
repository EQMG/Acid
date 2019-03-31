#include "Image.hpp"

#include "Renderer/Renderer.hpp"
#include "Renderer/Buffers/Buffer.hpp"
#include "Files/FileSystem.hpp"
#include "Files/Files.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace acid
{
static const float ANISOTROPY = 16.0f;

Image::Image(const VkExtent3D &extent, const VkImageType &imageType, const VkFormat &format, const VkSampleCountFlagBits &samples, const VkImageTiling &tiling,
	const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, const uint32_t &mipLevels, const uint32_t &arrayLayers) :
	m_extent(extent),
	m_format(format),
	m_samples(samples),
	m_usage(usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
	m_mipLevels(mipLevels),
	//m_arrayLayers(arrayLevels),
	//m_filter(filter),
	//m_addressMode(addressMode),
	//m_anisotropic(anisotropic),
	//m_layout(layout),
	m_image(VK_NULL_HANDLE),
	m_memory(VK_NULL_HANDLE),
	m_sampler(VK_NULL_HANDLE),
	m_view(VK_NULL_HANDLE)
{
	Image::CreateImage(m_image, m_memory, m_extent, m_format, m_samples, tiling, m_usage, properties, m_mipLevels, arrayLayers, imageType);
	//Image::CreateImageView(m_image, m_view, viewType, m_format, imageAspect, m_mipLevels, baseMipLevel, arrayLayers, baseArrayLayer);
	//Image::CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);
}

Image::~Image()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	vkDestroyImageView(logicalDevice->GetLogicalDevice(), m_view, nullptr);
	vkDestroySampler(logicalDevice->GetLogicalDevice(), m_sampler, nullptr);
	vkFreeMemory(logicalDevice->GetLogicalDevice(), m_memory, nullptr);
	vkDestroyImage(logicalDevice->GetLogicalDevice(), m_image, nullptr);
}

VkDescriptorSetLayoutBinding Image::GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage, const uint32_t &count)
{
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
	descriptorSetLayoutBinding.binding = binding;
	descriptorSetLayoutBinding.descriptorType = descriptorType;
	descriptorSetLayoutBinding.descriptorCount = 1;
	descriptorSetLayoutBinding.stageFlags = stage;
	descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
	return descriptorSetLayoutBinding;
}

WriteDescriptorSet Image::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const std::optional<OffsetSize> &offsetSize) const
{
	VkDescriptorImageInfo imageInfo = {};
	imageInfo.sampler = m_sampler;
	imageInfo.imageView = m_view;
	imageInfo.imageLayout = m_layout;

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

std::unique_ptr<uint8_t[]> Image::GetPixels(VkExtent3D &extent, const uint32_t &mipLevel, const uint32_t &arrayLayer) const
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	extent.width = int32_t(m_extent.width >> mipLevel);
	extent.height = int32_t(m_extent.height >> mipLevel);

	VkImage dstImage;
	VkDeviceMemory dstImageMemory;
	CopyImage(m_image, dstImage, dstImageMemory, m_format, m_extent, mipLevel, arrayLayer, false);

	VkImageSubresource dstImageSubresource = {};
	dstImageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	dstImageSubresource.mipLevel = 0;
	dstImageSubresource.arrayLayer = 0;

	VkSubresourceLayout dstSubresourceLayout;
	vkGetImageSubresourceLayout(logicalDevice->GetLogicalDevice(), dstImage, &dstImageSubresource, &dstSubresourceLayout);

	auto pixels = std::make_unique<uint8_t[]>(dstSubresourceLayout.size);

	void *data;
	vkMapMemory(logicalDevice->GetLogicalDevice(), dstImageMemory, dstSubresourceLayout.offset, dstSubresourceLayout.size, 0, &data);
	std::memcpy(pixels.get(), data, static_cast<size_t>(dstSubresourceLayout.size));
	vkUnmapMemory(logicalDevice->GetLogicalDevice(), dstImageMemory);

	vkFreeMemory(logicalDevice->GetLogicalDevice(), dstImageMemory, nullptr);
	vkDestroyImage(logicalDevice->GetLogicalDevice(), dstImage, nullptr);

	return pixels;
}

void Image::SetPixels(const uint8_t *pixels, const uint32_t &layerCount, const uint32_t &baseArrayLayer)
{
	Buffer bufferStaging = Buffer(m_extent.width * m_extent.height * 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void *data;
	bufferStaging.MapMemory(&data);
	std::memcpy(data, pixels, bufferStaging.GetSize());
	bufferStaging.UnmapMemory();

	CopyBufferToImage(bufferStaging.GetBuffer(), m_image, m_extent, layerCount, baseArrayLayer);
}

std::unique_ptr<uint8_t[]> Image::LoadPixels(const std::string &filename, uint32_t &width, uint32_t &height, uint32_t &components, VkFormat &format)
{
	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded)
	{
		Log::Error("Image could not be loaded: '%s'\n", filename.c_str());
		return nullptr;
	}

	std::unique_ptr<uint8_t[]> pixels(
		stbi_load_from_memory(reinterpret_cast<uint8_t *>(fileLoaded->data()), static_cast<uint32_t>(fileLoaded->size()), reinterpret_cast<int32_t *>(&width),
			reinterpret_cast<int32_t *>(&height), reinterpret_cast<int32_t *>(&components), STBI_rgb_alpha));

	// STBI_rgb_alpha converts the loaded image to a 32 bit image, if another loader is used components and format may differ.
	components = 4;
	format = VK_FORMAT_R8G8B8A8_UNORM;

	if (pixels == nullptr)
	{
		Log::Error("Unable to load Image: '%s'\n", filename.c_str());
	}

	return pixels;
}

void Image::WritePixels(const std::string &filename, const uint8_t *pixels, const int32_t &width, const int32_t &height, const int32_t &components)
{
	int32_t result = stbi_write_png(filename.c_str(), width, height, components, pixels, width * components);

	if (result != 1)
	{
		Log::Error("Unable to write pixels: '%s'\n", filename.c_str());
	}
}

uint32_t Image::GetMipLevels(const VkExtent3D &extent)
{
	return static_cast<uint32_t>(std::floor(std::log2(std::max(extent.width, std::max(extent.height, extent.depth)))) + 1);
}

bool Image::HasDepth(const VkFormat &format)
{
	static const std::vector<VkFormat> DEPTH_FORMATS = { VK_FORMAT_D16_UNORM, VK_FORMAT_X8_D24_UNORM_PACK32, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT };
	return std::find(DEPTH_FORMATS.begin(), DEPTH_FORMATS.end(), format) != std::end(DEPTH_FORMATS);
}

bool Image::HasStencil(const VkFormat &format)
{
	static const std::vector<VkFormat> STENCIL_FORMATS = { VK_FORMAT_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT };
	return std::find(STENCIL_FORMATS.begin(), STENCIL_FORMATS.end(), format) != std::end(STENCIL_FORMATS);
}

void Image::CreateImage(VkImage &image, VkDeviceMemory &memory, const VkExtent3D &extent, const VkFormat &format, const VkSampleCountFlagBits &samples, const VkImageTiling &tiling,
	const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, const uint32_t &mipLevels, const uint32_t &arrayLayers, const VkImageType &type)
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.flags = arrayLayers == 6 ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
	imageCreateInfo.imageType = type;
	imageCreateInfo.format = format;
	imageCreateInfo.extent = extent;
	imageCreateInfo.mipLevels = mipLevels;
	imageCreateInfo.arrayLayers = arrayLayers;
	imageCreateInfo.samples = samples;
	imageCreateInfo.tiling = tiling;
	imageCreateInfo.usage = usage;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	Renderer::CheckVk(vkCreateImage(logicalDevice->GetLogicalDevice(), &imageCreateInfo, nullptr, &image));

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(logicalDevice->GetLogicalDevice(), image, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = Buffer::FindMemoryType(memoryRequirements.memoryTypeBits, properties);
	Renderer::CheckVk(vkAllocateMemory(logicalDevice->GetLogicalDevice(), &memoryAllocateInfo, nullptr, &memory));

	Renderer::CheckVk(vkBindImageMemory(logicalDevice->GetLogicalDevice(), image, memory, 0));
}

void Image::CreateImageSampler(VkSampler &sampler, const VkFilter &filter, const VkSamplerAddressMode &addressMode, const bool &anisotropic, const uint32_t &mipLevels)
{
	auto physicalDevice = Renderer::Get()->GetPhysicalDevice();
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = filter;
	samplerCreateInfo.minFilter = filter;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU = addressMode;
	samplerCreateInfo.addressModeV = addressMode;
	samplerCreateInfo.addressModeW = addressMode;
	samplerCreateInfo.mipLodBias = 0.0f;
	samplerCreateInfo.anisotropyEnable = static_cast<VkBool32>(anisotropic);
	samplerCreateInfo.maxAnisotropy =
		(anisotropic && logicalDevice->GetEnabledFeatures().samplerAnisotropy) ? std::min(ANISOTROPY, physicalDevice->GetProperties().limits.maxSamplerAnisotropy) : 1.0f;
	//samplerCreateInfo.compareEnable = VK_FALSE;
	//samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = static_cast<float>(mipLevels);
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
	Renderer::CheckVk(vkCreateSampler(logicalDevice->GetLogicalDevice(), &samplerCreateInfo, nullptr, &sampler));
}

void Image::CreateImageView(const VkImage &image, VkImageView &imageView, const VkImageViewType &type, const VkFormat &format, const VkImageAspectFlags &imageAspect, const uint32_t &mipLevels,
	const uint32_t &baseMipLevel, const uint32_t &layerCount, const uint32_t &baseArrayLayer)
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.image = image;
	imageViewCreateInfo.viewType = type;
	imageViewCreateInfo.format = format;
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	imageViewCreateInfo.subresourceRange.aspectMask = imageAspect;
	imageViewCreateInfo.subresourceRange.baseMipLevel = baseMipLevel;
	imageViewCreateInfo.subresourceRange.levelCount = mipLevels;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
	imageViewCreateInfo.subresourceRange.layerCount = layerCount;
	Renderer::CheckVk(vkCreateImageView(logicalDevice->GetLogicalDevice(), &imageViewCreateInfo, nullptr, &imageView));
}

void Image::CreateMipmaps(const VkImage &image, const VkExtent3D &extent, const VkFormat &format, const VkImageLayout &dstImageLayout, const uint32_t &mipLevels,
	const uint32_t &baseArrayLayer, const uint32_t &layerCount)
{
	auto physicalDevice = Renderer::Get()->GetPhysicalDevice();

	// Get device properites for the requested texture format.
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(physicalDevice->GetPhysicalDevice(), format, &formatProperties);

	// Mip-chain generation requires support for blit source and destination
	assert(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT);
	assert(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT);

	CommandBuffer commandBuffer = CommandBuffer();

	for (uint32_t i = 1; i < mipLevels; i++)
	{
		VkImageMemoryBarrier barrier0 = {};
		barrier0.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier0.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier0.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier0.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier0.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier0.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier0.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier0.image = image;
		barrier0.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier0.subresourceRange.baseMipLevel = i - 1;
		barrier0.subresourceRange.levelCount = 1;
		barrier0.subresourceRange.baseArrayLayer = baseArrayLayer;
		barrier0.subresourceRange.layerCount = layerCount;
		vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier0);

		VkImageBlit imageBlit = {};
		imageBlit.srcOffsets[1] = { int32_t(extent.width >> (i - 1)), int32_t(extent.height >> (i - 1)), 1 };
		imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageBlit.srcSubresource.mipLevel = i - 1;
		imageBlit.srcSubresource.baseArrayLayer = baseArrayLayer;
		imageBlit.srcSubresource.layerCount = layerCount;
		imageBlit.dstOffsets[1] = { int32_t(extent.width >> i), int32_t(extent.height >> i), 1 };
		imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageBlit.dstSubresource.mipLevel = i;
		imageBlit.dstSubresource.baseArrayLayer = baseArrayLayer;
		imageBlit.dstSubresource.layerCount = layerCount;
		vkCmdBlitImage(commandBuffer.GetCommandBuffer(), image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);

		VkImageMemoryBarrier barrier1 = {};
		barrier1.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier1.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier1.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		barrier1.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier1.newLayout = dstImageLayout;
		barrier1.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier1.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier1.image = image;
		barrier1.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier1.subresourceRange.baseMipLevel = i - 1;
		barrier1.subresourceRange.levelCount = 1;
		barrier1.subresourceRange.baseArrayLayer = baseArrayLayer;
		barrier1.subresourceRange.layerCount = layerCount;
		vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier1);
	}

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = dstImageLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = baseArrayLayer;
	barrier.subresourceRange.layerCount = layerCount;
	vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	commandBuffer.End();
	commandBuffer.SubmitIdle();
}

void Image::TransitionImageLayout(const VkImage &image, const VkFormat &format, const VkImageLayout &srcImageLayout, const VkImageLayout &dstImageLayout,
	const VkImageAspectFlags &imageAspect, const uint32_t &mipLevels, const uint32_t &baseMipLevel, const uint32_t &layerCount, const uint32_t &baseArrayLayer)
{
	CommandBuffer commandBuffer = CommandBuffer();

	VkImageMemoryBarrier imageMemoryBarrier = {};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.oldLayout = srcImageLayout;
	imageMemoryBarrier.newLayout = dstImageLayout;
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.image = image;
	imageMemoryBarrier.subresourceRange.aspectMask = imageAspect;
	imageMemoryBarrier.subresourceRange.baseMipLevel = baseMipLevel;
	imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = baseArrayLayer;
	imageMemoryBarrier.subresourceRange.layerCount = layerCount;

	// Source access mask controls actions that have to be finished on the old layout before it will be transitioned to the new layout.
	switch (srcImageLayout)
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
	default:
		throw std::runtime_error("Unsupported image layout transition source");
	}

	// Destination access mask controls the dependency for the new image layout.
	switch (dstImageLayout)
	{
	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		break;
	case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
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
	default:
		throw std::runtime_error("Unsupported image layout transition destination");
	}

	VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
	VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

	vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(), srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	commandBuffer.End();
	commandBuffer.SubmitIdle();
}

void Image::InsertImageMemoryBarrier(const CommandBuffer &commandBuffer, const VkImage &image, const VkAccessFlags &srcAccessMask, const VkAccessFlags &dstAccessMask,
	const VkImageLayout &oldImageLayout, const VkImageLayout &newImageLayout, const VkPipelineStageFlags &srcStageMask, const VkPipelineStageFlags &dstStageMask,
	const VkImageAspectFlags &imageAspect, const uint32_t &mipLevels, const uint32_t &baseMipLevel, const uint32_t &layerCount, const uint32_t &baseArrayLayer)
{
	VkImageMemoryBarrier imageMemoryBarrier = {};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.srcAccessMask = srcAccessMask;
	imageMemoryBarrier.dstAccessMask = dstAccessMask;
	imageMemoryBarrier.oldLayout = oldImageLayout;
	imageMemoryBarrier.newLayout = newImageLayout;
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.image = image;
	imageMemoryBarrier.subresourceRange.aspectMask = imageAspect;
	imageMemoryBarrier.subresourceRange.baseMipLevel = baseMipLevel;
	imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = baseArrayLayer;
	imageMemoryBarrier.subresourceRange.layerCount = layerCount;
	vkCmdPipelineBarrier(commandBuffer.GetCommandBuffer(), srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
}

void Image::CopyBufferToImage(const VkBuffer &buffer, const VkImage &image, const VkExtent3D &extent, const uint32_t &layerCount, const uint32_t &baseArrayLayer)
{
	CommandBuffer commandBuffer = CommandBuffer();

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = baseArrayLayer;
	region.imageSubresource.layerCount = layerCount;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = extent;
	vkCmdCopyBufferToImage(commandBuffer.GetCommandBuffer(), buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	commandBuffer.End();
	commandBuffer.SubmitIdle();
}

bool Image::CopyImage(const VkImage &srcImage, VkImage &dstImage, VkDeviceMemory &dstImageMemory, const VkFormat &srcFormat, const VkExtent3D &extent,
	const uint32_t &mipLevel, const uint32_t &arrayLayer, const bool &srcSwapchain)
{
	auto physicalDevice = Renderer::Get()->GetPhysicalDevice();
	auto surface = Renderer::Get()->GetSurface();

	// Checks blit swapchain support.
	bool supportsBlit = true;
	VkFormatProperties formatProperties;

	// Check if the device supports blitting from optimal images (the swapchain images are in optimal format).
	vkGetPhysicalDeviceFormatProperties(physicalDevice->GetPhysicalDevice(), surface->GetFormat().format, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT))
	{
		Log::Error("Device does not support blitting from optimal tiled images, using copy instead of blit!\n");
		supportsBlit = false;
	}

	// Check if the device supports blitting to linear images.
	vkGetPhysicalDeviceFormatProperties(physicalDevice->GetPhysicalDevice(), srcFormat, &formatProperties);

	if (!(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT))
	{
		Log::Error("Device does not support blitting to linear tiled images, using copy instead of blit!\n");
		supportsBlit = false;
	}

	CreateImage(dstImage, dstImageMemory, extent, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 1, 1, VK_IMAGE_TYPE_2D);

	// Do the actual blit from the swapchain image to our host visible destination image.
	CommandBuffer commandBuffer = CommandBuffer();

	// Transition destination image to transfer destination layout.
	InsertImageMemoryBarrier(commandBuffer, dstImage, 0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, 
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 
		VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, 1, 0);

	// Transition swapchain image from present to transfer source layout
	if (srcSwapchain)
	{
		InsertImageMemoryBarrier(commandBuffer, srcImage, VK_ACCESS_MEMORY_READ_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 
			VK_IMAGE_ASPECT_COLOR_BIT, 1, mipLevel, 1, arrayLayer);
	}

	// If source and destination support blit we'll blit as this also does automatic format conversion (e.g. from BGR to RGB).
	if (supportsBlit)
	{
		// Define the region to blit (we will blit the whole swapchain image).
		VkOffset3D blitSize = { static_cast<int32_t>(extent.width), static_cast<int32_t>(extent.height), static_cast<int32_t>(extent.depth) };
		VkImageBlit imageBlitRegion = {};
		imageBlitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageBlitRegion.srcSubresource.mipLevel = mipLevel;
		imageBlitRegion.srcSubresource.baseArrayLayer = arrayLayer;
		imageBlitRegion.srcSubresource.layerCount = 1;
		imageBlitRegion.srcOffsets[1] = blitSize;
		imageBlitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageBlitRegion.dstSubresource.mipLevel = 0;
		imageBlitRegion.dstSubresource.baseArrayLayer = 0;
		imageBlitRegion.dstSubresource.layerCount = 1;
		imageBlitRegion.dstOffsets[1] = blitSize;
		vkCmdBlitImage(commandBuffer.GetCommandBuffer(), srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlitRegion,
			VK_FILTER_NEAREST);
	}
	else
	{
		// Otherwise use image copy (requires us to manually flip components).
		VkImageCopy imageCopyRegion = {};
		imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.srcSubresource.mipLevel = mipLevel;
		imageCopyRegion.srcSubresource.baseArrayLayer = arrayLayer;
		imageCopyRegion.srcSubresource.layerCount = 1;
		imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.dstSubresource.mipLevel = 0;
		imageCopyRegion.dstSubresource.baseArrayLayer = 0;
		imageCopyRegion.dstSubresource.layerCount = 1;
		imageCopyRegion.extent = extent;
		vkCmdCopyImage(commandBuffer.GetCommandBuffer(), srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
	}

	// Transition destination image to general layout, which is the required layout for mapping the image memory later on.
	InsertImageMemoryBarrier(commandBuffer, dstImage, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
		VK_IMAGE_LAYOUT_GENERAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, 1, 0);

	// Transition back the swap chain image after the blit is done.
	if (srcSwapchain)
	{
		InsertImageMemoryBarrier(commandBuffer, srcImage, VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_MEMORY_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 
			VK_IMAGE_ASPECT_COLOR_BIT, 1, mipLevel, 1, arrayLayer);
	}

	commandBuffer.End();
	commandBuffer.SubmitIdle();

	return supportsBlit;
}
}
