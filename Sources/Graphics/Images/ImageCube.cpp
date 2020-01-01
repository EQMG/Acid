#include "ImageCube.hpp"

#include "Bitmaps/Bitmap.hpp"
#include "Graphics/Buffers/Buffer.hpp"
#include "Graphics/Graphics.hpp"
#include "Resources/Resources.hpp"
#include "Image.hpp"

namespace acid {
std::shared_ptr<ImageCube> ImageCube::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<ImageCube>(node))
		return resource;

	auto result = std::make_shared<ImageCube>("");
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ImageCube> ImageCube::Create(const std::filesystem::path &filename, const std::string &fileSuffix, VkFilter filter, VkSamplerAddressMode addressMode,
	bool anisotropic, bool mipmap) {
	ImageCube temp(filename, fileSuffix, filter, addressMode, anisotropic, mipmap, false);
	Node node;
	node << temp;
	return Create(node);
}

ImageCube::ImageCube(std::filesystem::path filename, std::string fileSuffix, VkFilter filter, VkSamplerAddressMode addressMode, bool anisotropic, bool mipmap, bool load) :
	m_filename(std::move(filename)),
	m_fileSuffix(std::move(fileSuffix)),
	m_filter(filter),
	m_addressMode(addressMode),
	m_anisotropic(anisotropic),
	m_mipmap(mipmap),
	m_samples(VK_SAMPLE_COUNT_1_BIT),
	m_layout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
	m_usage(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
	m_format(VK_FORMAT_R8G8B8A8_UNORM) {
	if (load) {
		ImageCube::Load();
	}
}

ImageCube::ImageCube(const Vector2ui &extent, VkFormat format, VkImageLayout layout, VkImageUsageFlags usage, VkFilter filter, VkSamplerAddressMode addressMode, 
	VkSampleCountFlagBits samples, bool anisotropic, bool mipmap) :
	m_filter(filter),
	m_addressMode(addressMode),
	m_anisotropic(anisotropic),
	m_mipmap(mipmap),
	m_samples(samples),
	m_layout(layout),
	m_usage(usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
	m_format(format),
	m_components(4),
	m_extent(extent) {
	ImageCube::Load();
}

ImageCube::ImageCube(std::unique_ptr<Bitmap> &&bitmap, VkFormat format, VkImageLayout layout, VkImageUsageFlags usage, VkFilter filter, VkSamplerAddressMode addressMode, 
	VkSampleCountFlagBits samples, bool anisotropic, bool mipmap) :
	m_filter(filter),
	m_addressMode(addressMode),
	m_anisotropic(anisotropic),
	m_mipmap(mipmap),
	m_samples(samples),
	m_layout(layout),
	m_usage(usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
	m_format(format),
	m_components(bitmap->GetBytesPerPixel()),
	m_extent(bitmap->GetSize()) {
	ImageCube::Load(std::move(bitmap));
}

ImageCube::~ImageCube() {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	vkDestroyImageView(*logicalDevice, m_view, nullptr);
	vkDestroySampler(*logicalDevice, m_sampler, nullptr);
	vkFreeMemory(*logicalDevice, m_memory, nullptr);
	vkDestroyImage(*logicalDevice, m_image, nullptr);
}

WriteDescriptorSet ImageCube::GetWriteDescriptor(uint32_t binding, VkDescriptorType descriptorType, const std::optional<OffsetSize> &offsetSize) const {
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
	return {descriptorWrite, imageInfo};
}

VkDescriptorSetLayoutBinding ImageCube::GetDescriptorSetLayout(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stage,	uint32_t count) {
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
	descriptorSetLayoutBinding.binding = binding;
	descriptorSetLayoutBinding.descriptorType = descriptorType;
	descriptorSetLayoutBinding.descriptorCount = 1;
	descriptorSetLayoutBinding.stageFlags = stage;
	descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
	return descriptorSetLayoutBinding;
}

std::unique_ptr<Bitmap> ImageCube::GetBitmap(uint32_t mipLevel, uint32_t arrayLayer) const {
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto size = m_extent >> mipLevel;

	VkImage dstImage;
	VkDeviceMemory dstImageMemory;
	Image::CopyImage(m_image, dstImage, dstImageMemory, m_format, {size.m_x, size.m_y, 1}, m_layout, mipLevel, arrayLayer);

	VkImageSubresource dstImageSubresource = {};
	dstImageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	dstImageSubresource.mipLevel = 0;
	dstImageSubresource.arrayLayer = 0;

	VkSubresourceLayout dstSubresourceLayout;
	vkGetImageSubresourceLayout(*logicalDevice, dstImage, &dstImageSubresource, &dstSubresourceLayout);

	auto bitmap = std::make_unique<Bitmap>(std::make_unique<uint8_t[]>(dstSubresourceLayout.size), size);

	void *data;
	vkMapMemory(*logicalDevice, dstImageMemory, dstSubresourceLayout.offset, dstSubresourceLayout.size, 0, &data);
	std::memcpy(bitmap->GetData().get(), data, static_cast<std::size_t>(dstSubresourceLayout.size));
	vkUnmapMemory(*logicalDevice, dstImageMemory);

	vkFreeMemory(*logicalDevice, dstImageMemory, nullptr);
	vkDestroyImage(*logicalDevice, dstImage, nullptr);

	return bitmap;
}

std::unique_ptr<Bitmap> ImageCube::GetBitmap(uint32_t mipLevel) const {
	auto size = m_extent >> mipLevel;
	auto sizeSide = size.m_x * size.m_y * m_components;
	auto bitmap = std::make_unique<Bitmap>(Vector2ui(size.m_x, size.m_y * 6), m_components);
	auto offset = bitmap->GetData().get();

	for (uint32_t i = 0; i < 6; i++) {
		auto bitmapSide = GetBitmap(mipLevel, i);
		std::memcpy(offset, bitmapSide->GetData().get(), sizeSide);
		offset += sizeSide;
	}

	return bitmap;
}

void ImageCube::SetPixels(const uint8_t *pixels, uint32_t layerCount, uint32_t baseArrayLayer) {
	Buffer bufferStaging(m_extent.m_x * m_extent.m_y * m_components * 6, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void *data;
	bufferStaging.MapMemory(&data);
	memcpy(data, pixels, bufferStaging.GetSize());
	bufferStaging.UnmapMemory();

	Image::CopyBufferToImage(bufferStaging.GetBuffer(), m_image, {m_extent.m_x, m_extent.m_y, 1}, layerCount, baseArrayLayer);
}

const Node &operator>>(const Node &node, ImageCube &image) {
	node["filename"].Get(image.m_filename);
	node["fileSuffix"].Get(image.m_fileSuffix);
	node["fileSides"].Get(image.m_fileSides);
	node["filter"].Get(image.m_filter);
	node["addressMode"].Get(image.m_addressMode);
	node["anisotropic"].Get(image.m_anisotropic);
	node["mipmap"].Get(image.m_mipmap);
	return node;
}

Node &operator<<(Node &node, const ImageCube &image) {
	node["filename"].Set(image.m_filename);
	node["fileSuffix"].Set(image.m_fileSuffix);
	node["fileSides"].Set(image.m_fileSides);
	node["filter"].Set(image.m_filter);
	node["addressMode"].Set(image.m_addressMode);
	node["anisotropic"].Set(image.m_anisotropic);
	node["mipmap"].Set(image.m_mipmap);
	return node;
}

void ImageCube::Load(std::unique_ptr<Bitmap> loadBitmap) {
	if (!m_filename.empty() && !loadBitmap) {
		uint8_t *offset = nullptr;

		for (const auto &side : m_fileSides) {
			Bitmap bitmapSide(m_filename / (side + m_fileSuffix));
			auto lengthSide = bitmapSide.GetLength();

			if (!loadBitmap) {
				loadBitmap = std::make_unique<Bitmap>(std::make_unique<uint8_t[]>(lengthSide * 6), bitmapSide.GetSize(),
					bitmapSide.GetBytesPerPixel());
				offset = loadBitmap->GetData().get();
			}

			std::memcpy(offset, bitmapSide.GetData().get(), lengthSide);
			offset += lengthSide;
		}
		
		m_extent = loadBitmap->GetSize();
		m_components = loadBitmap->GetBytesPerPixel();
	}

	if (m_extent.m_x == 0 || m_extent.m_y == 0) {
		return;
	}

	m_mipLevels = m_mipmap ? Image::GetMipLevels({m_extent.m_x, m_extent.m_y, 1}) : 1;

	Image::CreateImage(m_image, m_memory, {m_extent.m_x, m_extent.m_y, 1}, m_format, m_samples, VK_IMAGE_TILING_OPTIMAL, m_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_mipLevels, 6, VK_IMAGE_TYPE_2D);
	Image::CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);
	Image::CreateImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_CUBE, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 6, 0);

	if (loadBitmap || m_mipmap) {
		Image::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 6, 0);
	}

	if (loadBitmap) {
		Buffer bufferStaging(loadBitmap->GetLength() * 6, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		uint8_t *data;
		bufferStaging.MapMemory(reinterpret_cast<void**>(&data));
		std::memcpy(data, loadBitmap->GetData().get(), bufferStaging.GetSize());
		bufferStaging.UnmapMemory();

		Image::CopyBufferToImage(bufferStaging.GetBuffer(), m_image, {m_extent.m_x, m_extent.m_y, 1}, 6, 0);
	}

	if (m_mipmap) {
		Image::CreateMipmaps(m_image, {m_extent.m_x, m_extent.m_y, 1}, m_format, m_layout, m_mipLevels, 0, 6);
	} else if (loadBitmap) {
		Image::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 6, 0);
	} else {
		Image::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 6, 0);
	}
}
}
