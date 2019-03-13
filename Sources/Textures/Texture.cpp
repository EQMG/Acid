#include "Texture.hpp"

#include "Files/FileSystem.hpp"
#include "Files/Files.hpp"
#include "Renderer/Buffers/Buffer.hpp"
#include "Renderer/Renderer.hpp"
#include "Resources/Resources.hpp"
#include "Serialized/Metadata.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace acid
{
static const std::string FALLBACK_PATH = "Undefined.png";
static const float ANISOTROPY = 16.0f;

std::shared_ptr<Texture> Texture::Create(const Metadata& metadata)
{
	auto resource = Resources::Get()->Find(metadata);

	if(resource != nullptr)
		{
			return std::dynamic_pointer_cast<Texture>(resource);
		}

	auto result = std::make_shared<Texture>("");
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	result->Decode(metadata);
	result->Load();
	return result;
}

std::shared_ptr<Texture> Texture::Create(const std::string& filename, const VkFilter& filter, const VkSamplerAddressMode& addressMode, const bool& anisotropic, const bool& mipmap)
{
	auto temp = Texture(filename, filter, addressMode, anisotropic, mipmap, false);
	Metadata metadata = Metadata();
	temp.Encode(metadata);
	return Create(metadata);
}

Texture::Texture(std::string filename, const VkFilter& filter, const VkSamplerAddressMode& addressMode, const bool& anisotropic, const bool& mipmap, const bool& load)
	: m_filename(std::move(filename)), m_filter(filter), m_addressMode(addressMode), m_anisotropic(anisotropic), m_mipmap(mipmap), m_samples(VK_SAMPLE_COUNT_1_BIT), m_layout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
	  m_usage(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT), m_components(0), m_width(0), m_height(0), m_loadPixels(nullptr), m_mipLevels(0), m_image(VK_NULL_HANDLE), m_memory(VK_NULL_HANDLE), m_view(VK_NULL_HANDLE),
	  m_sampler(VK_NULL_HANDLE), m_format(VK_FORMAT_R8G8B8A8_UNORM)
{
	if(load)
		{
			Texture::Load();
		}
}

Texture::Texture(const uint32_t& width, const uint32_t& height, std::unique_ptr<uint8_t[]> pixels, const VkFormat& format, const VkImageLayout& imageLayout, const VkImageUsageFlags& usage, const VkFilter& filter, const VkSamplerAddressMode& addressMode,
				 const VkSampleCountFlagBits& samples, const bool& anisotropic, const bool& mipmap)
	: m_filename(""), m_filter(filter), m_addressMode(addressMode), m_anisotropic(anisotropic), m_mipmap(mipmap), m_samples(samples), m_layout(imageLayout), m_usage(usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
	  m_components(4), m_width(width), m_height(height), m_loadPixels(std::move(pixels)), m_mipLevels(0), m_image(VK_NULL_HANDLE), m_memory(VK_NULL_HANDLE), m_view(VK_NULL_HANDLE), m_sampler(VK_NULL_HANDLE), m_format(format)
{
	Texture::Load();
}

Texture::~Texture()
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	vkDestroySampler(logicalDevice->GetLogicalDevice(), m_sampler, nullptr);
	vkDestroyImageView(logicalDevice->GetLogicalDevice(), m_view, nullptr);
	vkFreeMemory(logicalDevice->GetLogicalDevice(), m_memory, nullptr);
	vkDestroyImage(logicalDevice->GetLogicalDevice(), m_image, nullptr);
}

VkDescriptorSetLayoutBinding Texture::GetDescriptorSetLayout(const uint32_t& binding, const VkDescriptorType& descriptorType, const VkShaderStageFlags& stage, const uint32_t& count)
{
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
	descriptorSetLayoutBinding.binding = binding;
	descriptorSetLayoutBinding.descriptorType = descriptorType;
	descriptorSetLayoutBinding.descriptorCount = 1;
	descriptorSetLayoutBinding.stageFlags = stage;
	descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
	return descriptorSetLayoutBinding;
}

WriteDescriptorSet Texture::GetWriteDescriptor(const uint32_t& binding, const VkDescriptorType& descriptorType, const VkDescriptorSet& descriptorSet, const std::optional<OffsetSize>& offsetSize) const
{
	VkDescriptorImageInfo imageInfo = {};
	imageInfo.sampler = m_sampler;
	imageInfo.imageView = m_view;
	imageInfo.imageLayout = m_layout;

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.descriptorType = descriptorType;
	//	descriptorWrite.pImageInfo
	//=
	//&imageInfo;
	return WriteDescriptorSet(descriptorWrite, imageInfo);
}

void Texture::Load()
{
	if(!m_filename.empty() && m_loadPixels == nullptr)
		{
#if defined(ACID_VERBOSE)
			auto debugStart = Engine::GetTime();
#endif
			m_loadPixels = LoadPixels(m_filename, m_width, m_height, m_components, m_format);
#if defined(ACID_VERBOSE)
			auto debugEnd = Engine::GetTime();
			Log::Out("Texture '%s' loaded in %.3fms\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds<float>());
#endif
		}

	if(m_width == 0 && m_height == 0)
		{
			return;
		}

	auto logicalDevice = Renderer::Get()->GetLogicalDevice();
	m_mipLevels = m_mipmap ? GetMipLevels(m_width, m_height) : 1;

	CreateImage(m_image, m_memory, m_width, m_height, VK_IMAGE_TYPE_2D, m_samples, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL, m_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1);

	if(m_loadPixels != nullptr || m_mipmap)
		{
			TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1);
		}

	if(m_loadPixels != nullptr)
		{
			Buffer bufferStaging = Buffer(m_width * m_height * m_components, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			void* data;
			vkMapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory(), 0, bufferStaging.GetSize(), 0, &data);
			memcpy(data, m_loadPixels.get(), bufferStaging.GetSize());
			vkUnmapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory());

			CopyBufferToImage(bufferStaging.GetBuffer(), m_image, m_width, m_height, 0, 1);
		}

	if(m_mipmap)
		{
			CreateMipmaps(m_image, m_width, m_height, m_layout, m_mipLevels, 0, 1);
		}
	else if(m_loadPixels != nullptr)
		{
			TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1);
		}
	else
		{
			TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1);
		}

	CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);
	CreateImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_2D, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1);
	m_loadPixels = nullptr;
}

void Texture::Decode(const Metadata& metadata)
{
	metadata.GetChild("Filename", m_filename);
	metadata.GetChild("Filter", m_filter);
	metadata.GetChild("Address Mode", m_addressMode);
	metadata.GetChild("Anisotropic", m_anisotropic);
	metadata.GetChild("Mipmap", m_mipmap);
}

void Texture::Encode(Metadata& metadata) const
{
	metadata.SetChild("Filename", m_filename);
	metadata.SetChild("Filter", m_filter);
	metadata.SetChild("Address Mode", m_addressMode);
	metadata.SetChild("Anisotropic", m_anisotropic);
	metadata.SetChild("Mipmap", m_mipmap);
}

std::unique_ptr<uint8_t[]> Texture::GetPixels(uint32_t& width, uint32_t& height, const uint32_t& mipLevel) const
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	width = int32_t(m_width >> (mipLevel - 1));
	height = int32_t(m_height >> (mipLevel - 1));

	VkImage dstImage;
	VkDeviceMemory dstImageMemory;
	CopyImage(m_image, dstImage, dstImageMemory, m_format, width, height, false, mipLevel, 0, 1);

	VkImageSubresource dstImageSubresource = {};
	dstImageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	dstImageSubresource.mipLevel = 0;
	dstImageSubresource.arrayLayer = 0;

	VkSubresourceLayout dstSubresourceLayout;
	vkGetImageSubresourceLayout(logicalDevice->GetLogicalDevice(), dstImage, &dstImageSubresource, &dstSubresourceLayout);

	auto result = std::make_unique<uint8_t[]>(dstSubresourceLayout.size);

	void* data;
	vkMapMemory(logicalDevice->GetLogicalDevice(), dstImageMemory, dstSubresourceLayout.offset, dstSubresourceLayout.size, 0, &data);
	std::memcpy(result.get(), data,
				static_cast<size_t>(dstSubresourceLayout.size)); // TODO: Map directly into result.
	vkUnmapMemory(logicalDevice->GetLogicalDevice(), dstImageMemory);

	vkFreeMemory(logicalDevice->GetLogicalDevice(), dstImageMemory, nullptr);
	vkDestroyImage(logicalDevice->GetLogicalDevice(), dstImage, nullptr);

	return result;
}

void Texture::SetPixels(const uint8_t* pixels)
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	Buffer bufferStaging = Buffer(m_width * m_height * m_components, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory(), 0, bufferStaging.GetSize(), 0, &data);
	std::memcpy(data, pixels, bufferStaging.GetSize());
	vkUnmapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory());
}

std::unique_ptr<uint8_t[]> Texture::LoadPixels(const std::string& filename, uint32_t& width, uint32_t& height, uint32_t& components, VkFormat& format)
{
	auto fileLoaded = Files::Read(filename);

	if(!fileLoaded)
		{
			if(filename == FALLBACK_PATH)
				{
					return nullptr;
				}

			Log::Error("Texture could not be loaded: '%s'\n", filename.c_str());
			return LoadPixels(FALLBACK_PATH, width, height, components, format);
		}

	std::unique_ptr<uint8_t[]> data(stbi_load_from_memory(reinterpret_cast<uint8_t*>(fileLoaded->data()), static_cast<uint32_t>(fileLoaded->size()), reinterpret_cast<int32_t*>(&width), reinterpret_cast<int32_t*>(&height), reinterpret_cast<int32_t*>(&components), STBI_rgb_alpha));

	// STBI_rgb_alpha
	// converts
	// the
	// loaded
	// image
	// to
	// a
	// 32
	// bit
	// image,
	// if
	// another
	// loader
	// is
	// used
	// components
	// and
	// format
	// may
	// differ.
	components = 4;
	format = VK_FORMAT_R8G8B8A8_UNORM;

	if(data == nullptr)
		{
			Log::Error("Unable to load texture: '%s'\n", filename.c_str());
		}

	return data;
}

std::unique_ptr<uint8_t[]> Texture::LoadPixels(const std::string& filename, const std::string& fileSuffix, const std::vector<std::string>& fileSides, uint32_t& width, uint32_t& height, uint32_t& components, VkFormat& format)
{
	std::unique_ptr<uint8_t[]> result = nullptr;
	uint8_t* offset = nullptr;

	for(const auto& side : fileSides)
		{
			std::string filenameSide = std::string(filename).append("/").append(side).append(fileSuffix);
			auto resultSide = LoadPixels(filenameSide, width, height, components, format);
			int32_t sizeSide = width * height * components;

			if(result == nullptr)
				{
					result = std::make_unique<uint8_t[]>(sizeSide * fileSides.size());
					offset = result.get();
				}

			memcpy(offset, resultSide.get(), sizeSide);
			offset += sizeSide;
		}

	return result;
}

void Texture::WritePixels(const std::string& filename, const uint8_t* pixels, const int32_t& width, const int32_t& height, const int32_t& components)
{
	int32_t result = stbi_write_png(filename.c_str(), width, height, components, pixels, width * components);

	if(result != 1)
		{
			Log::Error("Unable to write pixels: '%s'\n", filename.c_str());
		}
}

uint32_t Texture::GetMipLevels(const uint32_t& width, const uint32_t& height)
{
	return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height))) + 1);
}

bool Texture::HasDepth(const VkFormat& format)
{
	static const std::vector<VkFormat> DEPTH_FORMATS = {
		VK_FORMAT_D16_UNORM, VK_FORMAT_X8_D24_UNORM_PACK32, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT,
	};
	return std::find(DEPTH_FORMATS.begin(), DEPTH_FORMATS.end(), format) != std::end(DEPTH_FORMATS);
}

bool Texture::HasStencil(const VkFormat& format)
{
	static const std::vector<VkFormat> STENCIL_FORMATS = {
		VK_FORMAT_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D32_SFLOAT_S8_UINT,
	};
	return std::find(STENCIL_FORMATS.begin(), STENCIL_FORMATS.end(), format) != std::end(STENCIL_FORMATS);
}

void Texture::CreateImage(VkImage& image, VkDeviceMemory& memory, const uint32_t& width, const uint32_t& height, const VkImageType& type, const VkSampleCountFlagBits& samples, const uint32_t& mipLevels, const VkFormat& format, const VkImageTiling& tiling,
						  const VkImageUsageFlags& usage, const VkMemoryPropertyFlags& properties, const uint32_t& arrayLayers)
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.flags = arrayLayers == 6 ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
	imageCreateInfo.imageType = type;
	imageCreateInfo.format = format;
	imageCreateInfo.extent = {width, height, 1};
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

bool Texture::HasStencilComponent(const VkFormat& format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void Texture::TransitionImageLayout(const VkImage& image, const VkFormat& format, const VkImageLayout& srcImageLayout, const VkImageLayout& dstImageLayout, const VkImageAspectFlags& aspectMask, const uint32_t& mipLevels, const uint32_t& baseArrayLayer, const uint32_t& layerCount)
{
	CommandBuffer commandBuffer = CommandBuffer();

	VkImageMemoryBarrier imageMemoryBarrier = {};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.oldLayout = srcImageLayout;
	imageMemoryBarrier.newLayout = dstImageLayout;
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.image = image;
	imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = baseArrayLayer;
	imageMemoryBarrier.subresourceRange.layerCount = layerCount;

	// Source
	// access
	// mask
	// controls
	// actions
	// that
	// have
	// to
	// be
	// finished
	// on
	// the
	// old
	// layout
	// before
	// it
	// will
	// be
	// transitioned
	// to
	// the
	// new
	// layout.
	switch(srcImageLayout)
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

	// Destination
	// access
	// mask
	// controls
	// the
	// dependency
	// for
	// the
	// new
	// image
	// layout.
	switch(dstImageLayout)
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
				if(imageMemoryBarrier.srcAccessMask == 0)
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

void Texture::CopyBufferToImage(const VkBuffer& buffer, const VkImage& image, const uint32_t& width, const uint32_t& height, const uint32_t& baseArrayLayer, const uint32_t& layerCount)
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
	region.imageOffset = {0, 0, 0};
	region.imageExtent = {width, height, 1};
	vkCmdCopyBufferToImage(commandBuffer.GetCommandBuffer(), buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	commandBuffer.End();
	commandBuffer.SubmitIdle();
}

void Texture::CreateMipmaps(const VkImage& image, const uint32_t& width, const uint32_t& height, const VkImageLayout& dstImageLayout, const uint32_t& mipLevels, const uint32_t& baseArrayLayer, const uint32_t& layerCount)
{
	CommandBuffer commandBuffer = CommandBuffer();

	for(uint32_t i = 1; i < mipLevels; i++)
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
			imageBlit.srcOffsets[0] = {0, 0, 0};
			imageBlit.srcOffsets[1] = {int32_t(width >> (i - 1)), int32_t(height >> (i - 1)), 1};
			imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.srcSubresource.mipLevel = i - 1;
			imageBlit.srcSubresource.baseArrayLayer = baseArrayLayer;
			imageBlit.srcSubresource.layerCount = layerCount;
			imageBlit.dstOffsets[0] = {0, 0, 0};
			imageBlit.dstOffsets[1] = {int32_t(width >> i), int32_t(height >> i), 1};
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
			barrier1.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
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

void Texture::CreateImageSampler(VkSampler& sampler, const VkFilter& filter, const VkSamplerAddressMode& addressMode, const bool& anisotropic, const uint32_t& mipLevels)
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
	samplerCreateInfo.maxAnisotropy = anisotropic ? std::min(ANISOTROPY, physicalDevice->GetProperties().limits.maxSamplerAnisotropy) : 1.0f;
	//	samplerCreateInfo.compareEnable
	//=
	// VK_FALSE;
	//	samplerCreateInfo.compareOp
	//=
	// VK_COMPARE_OP_ALWAYS;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = static_cast<float>(mipLevels);
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
	Renderer::CheckVk(vkCreateSampler(logicalDevice->GetLogicalDevice(), &samplerCreateInfo, nullptr, &sampler));
}

void Texture::CreateImageView(const VkImage& image, VkImageView& imageView, const VkImageViewType& type, const VkFormat& format, const VkImageAspectFlags& imageAspect, const uint32_t& mipLevels, const uint32_t& baseArrayLayer, const uint32_t& layerCount)
{
	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.image = image;
	imageViewCreateInfo.viewType = type;
	imageViewCreateInfo.format = format;
	imageViewCreateInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
	imageViewCreateInfo.subresourceRange = {};
	imageViewCreateInfo.subresourceRange.aspectMask = imageAspect;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = mipLevels;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
	imageViewCreateInfo.subresourceRange.layerCount = layerCount;
	Renderer::CheckVk(vkCreateImageView(logicalDevice->GetLogicalDevice(), &imageViewCreateInfo, nullptr, &imageView));
}

bool Texture::CopyImage(const VkImage& srcImage, VkImage& dstImage, VkDeviceMemory& dstImageMemory, const VkFormat& format, const uint32_t& width, const uint32_t& height, const bool& srcSwapchain, const uint32_t& mipLevel, const uint32_t& baseArrayLayer,
						const uint32_t& layerCount)
{
	auto physicalDevice = Renderer::Get()->GetPhysicalDevice();
	auto surface = Renderer::Get()->GetSurface();

	// Checks
	// blit
	// swapchain
	// support.
	bool supportsBlit = true;
	VkFormatProperties formatProperties;

	// Check
	// if
	// the
	// device
	// supports
	// blitting
	// from
	// optimal
	// images
	// (the
	// swapchain
	// images
	// are
	// in
	// optimal
	// format).
	vkGetPhysicalDeviceFormatProperties(physicalDevice->GetPhysicalDevice(), surface->GetFormat().format, &formatProperties);

	if(!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT))
		{
			supportsBlit = false;
		}

	// Check
	// if
	// the
	// device
	// supports
	// blitting
	// to
	// linear
	// images.
	vkGetPhysicalDeviceFormatProperties(physicalDevice->GetPhysicalDevice(), format, &formatProperties);

	if(!(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT))
		{
			supportsBlit = false;
		}

	CreateImage(dstImage, dstImageMemory, width, height, VK_IMAGE_TYPE_2D, VK_SAMPLE_COUNT_1_BIT, 1, format, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 1);

	// Do
	// the
	// actual
	// blit
	// from
	// the
	// swapchain
	// image
	// to
	// our
	// host
	// visible
	// destination
	// image.
	CommandBuffer commandBuffer = CommandBuffer();

	// Transition
	// destination
	// image
	// to
	// transfer
	// destination
	// layout.
	InsertImageMemoryBarrier(commandBuffer.GetCommandBuffer(), dstImage, 0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
							 VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

	// Transition
	// swapchain
	// image
	// from
	// present
	// to
	// transfer
	// source
	// layout
	if(srcSwapchain)
		{
			InsertImageMemoryBarrier(commandBuffer.GetCommandBuffer(), srcImage, VK_ACCESS_MEMORY_READ_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
									 VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, mipLevel, baseArrayLayer, layerCount});
		}

	// If
	// source
	// and
	// destination
	// support
	// blit
	// we'll
	// blit
	// as
	// this
	// also
	// does
	// automatic
	// format
	// conversion
	// (e.g.
	// from
	// BGR
	// to
	// RGB).
	if(supportsBlit)
		{
			// Define the region to blit (we will blit the whole swapchain image).
			VkOffset3D blitSize = {static_cast<int32_t>(width), static_cast<int32_t>(height), 1};
			VkImageBlit imageBlitRegion = {};
			imageBlitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlitRegion.srcSubresource.mipLevel = mipLevel - 1;
			imageBlitRegion.srcSubresource.baseArrayLayer = baseArrayLayer;
			imageBlitRegion.srcSubresource.layerCount = 1;
			imageBlitRegion.srcOffsets[1] = blitSize;
			imageBlitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlitRegion.dstSubresource.mipLevel = 0;
			imageBlitRegion.dstSubresource.baseArrayLayer = 0;
			imageBlitRegion.dstSubresource.layerCount = 1;
			imageBlitRegion.dstOffsets[1] = blitSize;
			vkCmdBlitImage(commandBuffer.GetCommandBuffer(), srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlitRegion, VK_FILTER_NEAREST);
		}
	else
		{
			// Otherwise use image copy (requires us to manually flip components).
			VkImageCopy imageCopyRegion = {};
			imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.srcSubresource.mipLevel = mipLevel - 1;
			imageCopyRegion.srcSubresource.baseArrayLayer = baseArrayLayer;
			imageCopyRegion.srcSubresource.layerCount = 1;
			imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.dstSubresource.mipLevel = 0;
			imageCopyRegion.dstSubresource.baseArrayLayer = 0;
			imageCopyRegion.dstSubresource.layerCount = 1;
			imageCopyRegion.extent = {width, height, 1};
			vkCmdCopyImage(commandBuffer.GetCommandBuffer(), srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
		}

	// Transition
	// destination
	// image
	// to
	// general
	// layout,
	// which
	// is
	// the
	// required
	// layout
	// for
	// mapping
	// the
	// image
	// memory
	// later
	// on.
	InsertImageMemoryBarrier(commandBuffer.GetCommandBuffer(), dstImage, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
							 VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

	// Transition
	// back
	// the
	// swap
	// chain
	// image
	// after
	// the
	// blit
	// is
	// done.
	if(srcSwapchain)
		{
			InsertImageMemoryBarrier(commandBuffer.GetCommandBuffer(), srcImage, VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_MEMORY_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
									 VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, mipLevel, baseArrayLayer, layerCount});
		}

	commandBuffer.End();
	commandBuffer.SubmitIdle();

	return supportsBlit;
}

void Texture::InsertImageMemoryBarrier(const VkCommandBuffer& cmdbuffer, const VkImage& image, const VkAccessFlags& srcAccessMask, const VkAccessFlags& dstAccessMask, const VkImageLayout& oldImageLayout, const VkImageLayout& newImageLayout,
									   const VkPipelineStageFlags& srcStageMask, const VkPipelineStageFlags& dstStageMask, const VkImageSubresourceRange& subresourceRange)
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
	imageMemoryBarrier.subresourceRange = subresourceRange;
	vkCmdPipelineBarrier(cmdbuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
}
}
