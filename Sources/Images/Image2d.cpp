#include "Image2d.hpp"

#include "Renderer/Renderer.hpp"
#include "Resources/Resources.hpp"
#include "Serialized/Metadata.hpp"
#include "Image.hpp"

namespace acid
{
	static const std::string FALLBACK_PATH = "Undefined.png";
	static const float ANISOTROPY = 16.0f;

	std::shared_ptr<Image2d> Image2d::Create(const Metadata &metadata)
	{
		auto resource = Resources::Get()->Find(metadata);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<Image2d>(resource);
		}

		auto result = std::make_shared<Image2d>("");
		Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<Image2d> Image2d::Create(const std::string &filename, const VkFilter &filter, const VkSamplerAddressMode &addressMode, const bool &anisotropic, const bool &mipmap)
	{
		auto temp = Image2d(filename, filter, addressMode, anisotropic, mipmap, false);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	Image2d::Image2d(std::string filename, const VkFilter &filter, const VkSamplerAddressMode &addressMode, const bool &anisotropic, const bool &mipmap, const bool &load) :
		m_filename(std::move(filename)),
		m_filter(filter),
		m_addressMode(addressMode),
		m_anisotropic(anisotropic),
		m_mipmap(mipmap),
		m_samples(VK_SAMPLE_COUNT_1_BIT),
		m_layout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
		m_usage(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
		m_components(0),
		m_width(0),
		m_height(0),
		m_loadPixels(nullptr),
		m_mipLevels(0),
		m_image(VK_NULL_HANDLE),
		m_memory(VK_NULL_HANDLE),
		m_view(VK_NULL_HANDLE),
		m_sampler(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM)
	{
		if (load)
		{
			Image2d::Load();
		}
	}

	Image2d::Image2d(const uint32_t &width, const uint32_t &height, std::unique_ptr<uint8_t[]> pixels, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage, 
		const VkFilter &filter, const VkSamplerAddressMode &addressMode, const VkSampleCountFlagBits &samples, const bool &anisotropic, const bool &mipmap) :
		m_filename(""),
		m_filter(filter),
		m_addressMode(addressMode),
		m_anisotropic(anisotropic),
		m_mipmap(mipmap),
		m_samples(samples),
		m_layout(imageLayout),
		m_usage(usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
		m_components(4),
		m_width(width),
		m_height(height),
		m_loadPixels(std::move(pixels)),
		m_mipLevels(0),
		m_image(VK_NULL_HANDLE),
		m_memory(VK_NULL_HANDLE),
		m_view(VK_NULL_HANDLE),
		m_sampler(VK_NULL_HANDLE),
		m_format(format)
	{
		Image2d::Load();
	}

	Image2d::~Image2d()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		vkDestroySampler(logicalDevice->GetLogicalDevice(), m_sampler, nullptr);
		vkDestroyImageView(logicalDevice->GetLogicalDevice(), m_view, nullptr);
		vkFreeMemory(logicalDevice->GetLogicalDevice(), m_memory, nullptr);
		vkDestroyImage(logicalDevice->GetLogicalDevice(), m_image, nullptr);
	}

	VkDescriptorSetLayoutBinding Image2d::GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage, const uint32_t &count)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = stage;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		return descriptorSetLayoutBinding;
	}

	WriteDescriptorSet Image2d::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkDescriptorSet &descriptorSet,
		const std::optional<OffsetSize> &offsetSize) const
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
	//	descriptorWrite.pImageInfo = &imageInfo;
		return WriteDescriptorSet(descriptorWrite, imageInfo);
	}

	void Image2d::Load()
	{
		if (!m_filename.empty() && m_loadPixels == nullptr)
		{
#if defined(ACID_VERBOSE)
			auto debugStart = Engine::GetTime();
#endif
			m_loadPixels = Image::LoadPixels(m_filename, m_width, m_height, m_components, m_format);
#if defined(ACID_VERBOSE)
			auto debugEnd = Engine::GetTime();
			Log::Out("Image 2D '%s' loaded in %.3fms\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds<float>());
#endif
		}

		if (m_width == 0 && m_height == 0)
		{
			return;
		}

		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		m_mipLevels = m_mipmap ? Image::GetMipLevels(m_width, m_height) : 1;

		Image::CreateImage(m_image, m_memory, m_width, m_height, VK_IMAGE_TYPE_2D, m_samples, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL, m_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1);
		Image::CreateImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_2D, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1, 0);
		Image::CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);

		if (m_loadPixels != nullptr || m_mipmap)
		{
			Image::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1);
		}

		if (m_loadPixels != nullptr)
		{
			auto bufferStaging = Buffer(m_width * m_height * m_components, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			void *data;
			vkMapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory(), 0, bufferStaging.GetSize(), 0, &data);
			memcpy(data, m_loadPixels.get(), bufferStaging.GetSize());
			vkUnmapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory());

			Image::CopyBufferToImage(bufferStaging.GetBuffer(), m_image, m_width, m_height, 0, 1);
		}

		if (m_mipmap)
		{
			Image::CreateMipmaps(m_image, m_width, m_height, m_layout, m_mipLevels, 0, 1);
		}
		else if (m_loadPixels != nullptr)
		{
			Image::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1);
		}
		else
		{
			Image::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1);
		}

		m_loadPixels = nullptr;
	}

	void Image2d::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Filename", m_filename);
		metadata.GetChild("Filter", m_filter);
		metadata.GetChild("Address Mode", m_addressMode);
		metadata.GetChild("Anisotropic", m_anisotropic);
		metadata.GetChild("Mipmap", m_mipmap);
	}

	void Image2d::Encode(Metadata &metadata) const
	{
		metadata.SetChild("Filename", m_filename);
		metadata.SetChild("Filter", m_filter);
		metadata.SetChild("Address Mode", m_addressMode);
		metadata.SetChild("Anisotropic", m_anisotropic);
		metadata.SetChild("Mipmap", m_mipmap);
	}

	std::unique_ptr<uint8_t[]> Image2d::GetPixels(uint32_t &width, uint32_t &height, const uint32_t &mipLevel) const
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		
		width = int32_t(m_width >> (mipLevel - 1));
		height = int32_t(m_height >> (mipLevel - 1));

		VkImage dstImage;
		VkDeviceMemory dstImageMemory;
		Image::CopyImage(m_image, dstImage, dstImageMemory, m_format, width, height, false, mipLevel, 0, 1);

		VkImageSubresource dstImageSubresource = {};
		dstImageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		dstImageSubresource.mipLevel = 0;
		dstImageSubresource.arrayLayer = 0;

		VkSubresourceLayout dstSubresourceLayout;
		vkGetImageSubresourceLayout(logicalDevice->GetLogicalDevice(), dstImage, &dstImageSubresource, &dstSubresourceLayout);
		
		auto result = std::make_unique<uint8_t[]>(dstSubresourceLayout.size);

		void *data;
		vkMapMemory(logicalDevice->GetLogicalDevice(), dstImageMemory, dstSubresourceLayout.offset, dstSubresourceLayout.size, 0, &data);
		std::memcpy(result.get(), data, static_cast<size_t>(dstSubresourceLayout.size)); // TODO: Map directly into result.
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), dstImageMemory);

		vkFreeMemory(logicalDevice->GetLogicalDevice(), dstImageMemory, nullptr);
		vkDestroyImage(logicalDevice->GetLogicalDevice(), dstImage, nullptr);

		return result;
	}

	void Image2d::SetPixels(const uint8_t *pixels)
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		Buffer bufferStaging = Buffer(m_width * m_height * m_components, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory(), 0, bufferStaging.GetSize(), 0, &data);
		std::memcpy(data, pixels, bufferStaging.GetSize());
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory());
	}
}
