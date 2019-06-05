#include "Image2d.hpp"

#include "Graphics/Buffers/Buffer.hpp"
#include "Graphics/Graphics.hpp"
#include "Resources/Resources.hpp"
#include "Serialized/Metadata.hpp"
#include "Image.hpp"

namespace acid
{
std::shared_ptr<Image2d> Image2d::Create(const Metadata &metadata)
{
	auto resource = Resources::Get()->Find(metadata);

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<Image2d>(resource);
	}

	auto result = std::make_shared<Image2d>("");
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;
}

std::shared_ptr<Image2d> Image2d::Create(const std::string &filename, const VkFilter &filter, const VkSamplerAddressMode &addressMode, const bool &anisotropic, const bool &mipmap)
{
	Image2d temp{filename, filter, addressMode, anisotropic, mipmap, false};
	Metadata metadata;
	metadata << temp;
	return Create(metadata);
}

Image2d::Image2d(std::string filename, const VkFilter &filter, const VkSamplerAddressMode &addressMode, const bool &anisotropic, const bool &mipmap, const bool &load) :
	m_filename{std::move(filename)},
	m_filter{filter},
	m_addressMode{addressMode},
	m_anisotropic{anisotropic},
	m_mipmap{mipmap},
	m_samples{VK_SAMPLE_COUNT_1_BIT},
	m_layout{VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL},
	m_usage{VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT},
	m_format{VK_FORMAT_R8G8B8A8_UNORM}
{
	if (load)
	{
		Image2d::Load();
	}
}

Image2d::Image2d(const Vector2ui &extent, std::unique_ptr<uint8_t[]> pixels, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage,
	const VkFilter &filter, const VkSamplerAddressMode &addressMode, const VkSampleCountFlagBits &samples, const bool &anisotropic, const bool &mipmap) :
	m_filter{filter},
	m_addressMode{addressMode},
	m_anisotropic{anisotropic},
	m_mipmap{mipmap},
	m_samples{samples},
	m_layout{imageLayout},
	m_usage{usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT},
	m_components{4},
	m_extent{extent},
	m_loadPixels{std::move(pixels)},
	m_format{format}
{
	Image2d::Load();
}

Image2d::~Image2d()
{
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	vkDestroySampler(*logicalDevice, m_sampler, nullptr);
	vkDestroyImageView(*logicalDevice, m_view, nullptr);
	vkFreeMemory(*logicalDevice, m_memory, nullptr);
	vkDestroyImage(*logicalDevice, m_image, nullptr);
}

VkDescriptorSetLayoutBinding Image2d::GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage,
	const uint32_t &count)
{
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding{};
	descriptorSetLayoutBinding.binding = binding;
	descriptorSetLayoutBinding.descriptorType = descriptorType;
	descriptorSetLayoutBinding.descriptorCount = 1;
	descriptorSetLayoutBinding.stageFlags = stage;
	descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
	return descriptorSetLayoutBinding;
}

WriteDescriptorSet Image2d::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const std::optional<OffsetSize> &offsetSize) const
{
	VkDescriptorImageInfo imageInfo{};
	imageInfo.sampler = m_sampler;
	imageInfo.imageView = m_view;
	imageInfo.imageLayout = m_layout;

	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = VK_NULL_HANDLE; // Will be set in the descriptor handler.
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.descriptorType = descriptorType;
	//descriptorWrite.pImageInfo = &imageInfo;
	return WriteDescriptorSet(descriptorWrite, imageInfo);
}

void Image2d::Load()
{
	if (!m_filename.empty() && m_loadPixels == nullptr)
	{
#if defined(ACID_VERBOSE)
		auto debugStart = Time::Now();
#endif
		m_loadPixels = Image::LoadPixels(m_filename, m_extent, m_components, m_format);
#if defined(ACID_VERBOSE)
		auto debugEnd = Time::Now();
		Log::Out("Image 2D '%s' loaded in %.3fms\n", m_filename, (debugEnd - debugStart).AsMilliseconds<float>());
#endif
	}

	if (m_extent.m_x == 0 || m_extent.m_y == 0)
	{
		return;
	}

	m_mipLevels = m_mipmap ? Image::GetMipLevels({ m_extent.m_x, m_extent.m_y, 1 }) : 1;

	Image::CreateImage(m_image, m_memory, { m_extent.m_x, m_extent.m_y, 1 }, m_format, m_samples, VK_IMAGE_TILING_OPTIMAL, m_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_mipLevels, 1, VK_IMAGE_TYPE_2D);
	Image::CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);
	Image::CreateImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_2D, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1, 0);

	if (m_loadPixels != nullptr || m_mipmap)
	{
		//m_image.TransitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		Image::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1, 0);
	}

	if (m_loadPixels != nullptr)
	{
		//m_image.SetPixels(m_loadPixels.get(), 1, 0);
		Buffer bufferStaging{m_extent.m_x * m_extent.m_y * m_components, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

		void *data;
		bufferStaging.MapMemory(&data);
		std::memcpy(data, m_loadPixels.get(), bufferStaging.GetSize());
		bufferStaging.UnmapMemory();

		Image::CopyBufferToImage(bufferStaging.GetBuffer(), m_image, { m_extent.m_x, m_extent.m_y, 1 }, 1, 0);
	}

	if (m_mipmap)
	{
		//m_image.CreateMipmaps();
		Image::CreateMipmaps(m_image, { m_extent.m_x, m_extent.m_y, 1 }, m_format, m_layout, m_mipLevels, 0, 1);
	}
	else if (m_loadPixels != nullptr)
	{
		//m_image.TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_layout);
		Image::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1, 0);
	}
	else
	{
		//m_image.TransitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, m_layout);
		Image::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1, 0);
	}

	m_loadPixels = nullptr;
}

std::unique_ptr<uint8_t[]> Image2d::GetPixels(Vector2ui &extent, const uint32_t &mipLevel) const
{
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	extent = m_extent >> mipLevel;

	VkImage dstImage;
	VkDeviceMemory dstImageMemory;
	Image::CopyImage(m_image, dstImage, dstImageMemory, m_format, { extent.m_x, extent.m_y, 1 }, m_layout, mipLevel, 0);

	VkImageSubresource dstImageSubresource{};
	dstImageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	dstImageSubresource.mipLevel = 0;
	dstImageSubresource.arrayLayer = 0;

	VkSubresourceLayout dstSubresourceLayout;
	vkGetImageSubresourceLayout(*logicalDevice, dstImage, &dstImageSubresource, &dstSubresourceLayout);

	auto pixels = std::make_unique<uint8_t[]>(dstSubresourceLayout.size);

	void *data;
	vkMapMemory(*logicalDevice, dstImageMemory, dstSubresourceLayout.offset, dstSubresourceLayout.size, 0, &data);
	std::memcpy(pixels.get(), data, static_cast<size_t>(dstSubresourceLayout.size));
	vkUnmapMemory(*logicalDevice, dstImageMemory);

	vkFreeMemory(*logicalDevice, dstImageMemory, nullptr);
	vkDestroyImage(*logicalDevice, dstImage, nullptr);

	return pixels;
}

void Image2d::SetPixels(const uint8_t *pixels, const uint32_t &layerCount, const uint32_t &baseArrayLayer)
{
	Buffer bufferStaging{m_extent.m_x * m_extent.m_y * m_components, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

	void *data;
	bufferStaging.MapMemory(&data);
	std::memcpy(data, pixels, bufferStaging.GetSize());
	bufferStaging.UnmapMemory();

	Image::CopyBufferToImage(bufferStaging.GetBuffer(), m_image, { m_extent.m_x, m_extent.m_y, 1 }, layerCount, baseArrayLayer);
}

const Metadata &operator>>(const Metadata &metadata, Image2d &image)
{
	metadata.GetChild("Filename", image.m_filename);
	metadata.GetChild("Filter", image.m_filter);
	metadata.GetChild("Address Mode", image.m_addressMode);
	metadata.GetChild("Anisotropic", image.m_anisotropic);
	metadata.GetChild("Mipmap", image.m_mipmap);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Image2d &image)
{
	metadata.SetChild("Filename", image.m_filename);
	metadata.SetChild("Filter", image.m_filter);
	metadata.SetChild("Address Mode", image.m_addressMode);
	metadata.SetChild("Anisotropic", image.m_anisotropic);
	metadata.SetChild("Mipmap", image.m_mipmap);
	return metadata;
}
}
