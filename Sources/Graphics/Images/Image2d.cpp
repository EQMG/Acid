#include "Image2d.hpp"

#include <cstring>

#include "Bitmaps/Bitmap.hpp"
#include "Graphics/Buffers/Buffer.hpp"
#include "Graphics/Graphics.hpp"
#include "Resources/Resources.hpp"
#include "Files/Node.hpp"
#include "Image.hpp"

namespace acid {
std::shared_ptr<Image2d> Image2d::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<Image2d>(node))
		return resource;

	auto result = std::make_shared<Image2d>("");
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<Image2d> Image2d::Create(const std::filesystem::path &filename, VkFilter filter, VkSamplerAddressMode addressMode, bool anisotropic, bool mipmap) {
	Image2d temp(filename, filter, addressMode, anisotropic, mipmap, false);
	Node node;
	node << temp;
	return Create(node);
}

Image2d::Image2d(std::filesystem::path filename, VkFilter filter, VkSamplerAddressMode addressMode, bool anisotropic, bool mipmap, bool load) :
	Image(filter, addressMode, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_FORMAT_R8G8B8A8_UNORM, 1, 1, {0, 0, 1}),
	m_filename(std::move(filename)),
	m_anisotropic(anisotropic),
	m_mipmap(mipmap) {
	if (load) {
		Image2d::Load();
	}
}

Image2d::Image2d(const Vector2ui &extent, VkFormat format, VkImageLayout layout, VkImageUsageFlags usage, VkFilter filter, VkSamplerAddressMode addressMode,
	VkSampleCountFlagBits samples, bool anisotropic, bool mipmap) :
	Image(filter, addressMode, samples, layout, 
		usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		format, 1, 1, {extent.m_x, extent.m_y, 1}),
	m_anisotropic(anisotropic),
	m_mipmap(mipmap),
	m_components(4) {
	Image2d::Load();
}

Image2d::Image2d(std::unique_ptr<Bitmap> &&bitmap, VkFormat format, VkImageLayout layout, VkImageUsageFlags usage, VkFilter filter, VkSamplerAddressMode addressMode,
	VkSampleCountFlagBits samples, bool anisotropic, bool mipmap) :
	Image(filter, addressMode, samples, layout,
		usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		format, 1, 1, {bitmap->GetSize().m_x, bitmap->GetSize().m_y, 1}),
	m_anisotropic(anisotropic),
	m_mipmap(mipmap),
	m_components(bitmap->GetBytesPerPixel()) {
	Image2d::Load(std::move(bitmap));
}

void Image2d::SetPixels(const uint8_t *pixels, uint32_t layerCount, uint32_t baseArrayLayer) {
	Buffer bufferStaging(m_extent.width * m_extent.height * m_components, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void *data;
	bufferStaging.MapMemory(&data);
	std::memcpy(data, pixels, bufferStaging.GetSize());
	bufferStaging.UnmapMemory();

	CopyBufferToImage(bufferStaging.GetBuffer(), m_image, m_extent, layerCount, baseArrayLayer);
}

const Node &operator>>(const Node &node, Image2d &image) {
	node["filename"].Get(image.m_filename);
	node["filter"].Get(image.m_filter);
	node["addressMode"].Get(image.m_addressMode);
	node["anisotropic"].Get(image.m_anisotropic);
	node["mipmap"].Get(image.m_mipmap);
	return node;
}

Node &operator<<(Node &node, const Image2d &image) {
	node["filename"].Set(image.m_filename);
	node["filter"].Set(image.m_filter);
	node["addressMode"].Set(image.m_addressMode);
	node["anisotropic"].Set(image.m_anisotropic);
	node["mipmap"].Set(image.m_mipmap);
	return node;
}

void Image2d::Load(std::unique_ptr<Bitmap> loadBitmap) {
	if (!m_filename.empty() && !loadBitmap) {
		loadBitmap = std::make_unique<Bitmap>(m_filename);
		m_extent = {loadBitmap->GetSize().m_y, loadBitmap->GetSize().m_y, 1};
		m_components = loadBitmap->GetBytesPerPixel();
	}
		
	if (m_extent.width == 0 || m_extent.height == 0) {
		return;
	}

	m_mipLevels = m_mipmap ? GetMipLevels(m_extent) : 1;

	CreateImage(m_image, m_memory, m_extent, m_format, m_samples, VK_IMAGE_TILING_OPTIMAL, m_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_mipLevels, 1, VK_IMAGE_TYPE_2D);
	CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);
	CreateImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_2D, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1, 0);

	if (loadBitmap || m_mipmap) {
		TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1, 0);
	}

	if (loadBitmap) {
		Buffer bufferStaging(loadBitmap->GetLength(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		uint8_t *data;
		bufferStaging.MapMemory(reinterpret_cast<void **>(&data));
		std::memcpy(data, loadBitmap->GetData().get(), bufferStaging.GetSize());
		bufferStaging.UnmapMemory();

		CopyBufferToImage(bufferStaging.GetBuffer(), m_image, m_extent, 1, 0);
	}

	if (m_mipmap) {
		CreateMipmaps(m_image, m_extent, m_format, m_layout, m_mipLevels, 0, 1);
	} else if (loadBitmap) {
		TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1, 0);
	} else {
		TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1, 0);
	}
}
}
