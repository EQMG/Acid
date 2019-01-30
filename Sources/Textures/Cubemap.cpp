#include "Cubemap.hpp"

#include "Renderer/Renderer.hpp"
#include "Helpers/String.hpp"
#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Renderer/Buffers/Buffer.hpp"
#include "Texture.hpp"

namespace acid
{
	std::shared_ptr<Cubemap> Cubemap::Create(const Metadata &metadata)
	{
		auto result = std::make_shared<Cubemap>("");
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<Cubemap> Cubemap::Create(const std::string &filename, const std::string &fileSuffix, const VkFilter &filter, const VkSamplerAddressMode &addressMode,
		const bool &anisotropic, const bool &mipmap)
	{
		auto temp = Cubemap(filename, fileSuffix, filter, addressMode, anisotropic, mipmap);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	Cubemap::Cubemap(const std::string &filename, const std::string &fileSuffix, const VkFilter &filter, const VkSamplerAddressMode &addressMode,
		const bool &anisotropic, const bool &mipmap) :
		m_filename(filename),
		m_fileSuffix(fileSuffix),
		m_fileSides(std::vector<std::string>{"Right", "Left", "Top", "Bottom", "Back", "Front"}),
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
		m_pixels(nullptr),
		m_image(VK_NULL_HANDLE),
		m_memory(VK_NULL_HANDLE),
		m_view(VK_NULL_HANDLE),
		m_sampler(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM)
	{
		Load();
	}

	Cubemap::Cubemap(const uint32_t &width, const uint32_t &height, uint8_t *pixels, const VkFormat &format, const VkImageLayout &layout, const VkImageUsageFlags &usage,
		const VkFilter &filter, const VkSamplerAddressMode &addressMode, const VkSampleCountFlagBits &samples, const bool &anisotropic, const bool &mipmap) :
		m_filename(""),
		m_fileSuffix(""),
		m_fileSides(std::vector<std::string>{"Right", "Left", "Top", "Bottom", "Back", "Front"}),
		m_filter(filter),
		m_addressMode(addressMode),
		m_anisotropic(anisotropic),
		m_mipmap(mipmap),
		m_samples(samples),
		m_layout(layout),
		m_usage(usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
		m_components(4),
		m_width(width),
		m_height(height),
		m_pixels(pixels),
		m_image(VK_NULL_HANDLE),
		m_memory(VK_NULL_HANDLE),
		m_view(VK_NULL_HANDLE),
		m_sampler(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM)
	{
		Load();
	}

	Cubemap::~Cubemap()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		vkDestroySampler(logicalDevice->GetLogicalDevice(), m_sampler, nullptr);
		vkDestroyImageView(logicalDevice->GetLogicalDevice(), m_view, nullptr);
		vkFreeMemory(logicalDevice->GetLogicalDevice(), m_memory, nullptr);
		vkDestroyImage(logicalDevice->GetLogicalDevice(), m_image, nullptr);
	}

	VkDescriptorSetLayoutBinding Cubemap::GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage, const uint32_t &count)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = stage;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		return descriptorSetLayoutBinding;
	}

	WriteDescriptorSet Cubemap::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType,
		const VkDescriptorSet &descriptorSet, const std::optional<OffsetSize> &offsetSize) const
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

	void Cubemap::Load()
	{
		if (!m_filename.empty() && m_pixels == nullptr)
		{
#if defined(ACID_VERBOSE)
			auto debugStart = Engine::GetTime();
#endif
			m_pixels = Texture::LoadPixels(m_filename, m_fileSuffix, m_fileSides, &m_width, &m_height, &m_components);
#if defined(ACID_VERBOSE)
			auto debugEnd = Engine::GetTime();
			Log::Out("Cubemap '%s' loaded in %ims\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif
		}

		if (m_width == 0 && m_height == 0)
		{
			return;
		}

		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		auto mipLevels = m_mipmap ? Texture::GetMipLevels(m_width, m_height) : 1;

		Texture::CreateImage(m_image, m_memory, m_width, m_height, VK_IMAGE_TYPE_2D, m_samples, mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL,
			m_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 6);

		if (m_pixels != nullptr || m_mipmap)
		{
			Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels, 0, 6);
		}

		if (m_pixels != nullptr)
		{
			Buffer bufferStaging = Buffer(m_width * m_height * 4 * 6, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			void *data;
			Renderer::CheckVk(vkMapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory(), 0, bufferStaging.GetSize(), 0, &data));
			memcpy(data, m_pixels, bufferStaging.GetSize());
			vkUnmapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory());

			Texture::CopyBufferToImage(bufferStaging.GetBuffer(), m_image, m_width, m_height, 0, 6);
		}

		if (m_mipmap)
		{
			Texture::CreateMipmaps(m_image, m_width, m_height, m_layout, mipLevels, 0, 6);
		}
		else if (m_pixels != nullptr)
		{
			Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_layout, mipLevels, 0, 6);
		}
		else
		{
			Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, m_layout, mipLevels, 0, 6);
		}

		Texture::CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, mipLevels);
		Texture::CreateImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_CUBE, m_format, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels, 0, 6);
		Texture::DeletePixels(m_pixels);
	}

	void Cubemap::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Filename", m_filename);
		metadata.GetChild("Suffix", m_fileSuffix);
	//	metadata.GetChild("Sides", m_fileSides);
		metadata.GetChild("Filter", m_filter);
		metadata.GetChild("Address Mode", m_addressMode);
		metadata.GetChild("Anisotropic", m_anisotropic);
		metadata.GetChild("Mipmap", m_mipmap);
	}

	void Cubemap::Encode(Metadata &metadata) const
	{
		metadata.SetChild("Filename", m_filename);
		metadata.SetChild("Suffix", m_fileSuffix);
	//	metadata.SetChild("Sides", m_fileSides);
		metadata.SetChild("Filter", m_filter);
		metadata.SetChild("Address Mode", m_addressMode);
		metadata.SetChild("Anisotropic", m_anisotropic);
		metadata.SetChild("Mipmap", m_mipmap);
	}

	uint8_t *Cubemap::GetPixels(const uint32_t &arrayLayer) const
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		VkImage dstImage;
		VkDeviceMemory dstImageMemory;
		Texture::CopyImage(m_image, dstImage, dstImageMemory, m_width, m_height, false, arrayLayer, 6);

		VkImageSubresource imageSubresource = {};
		imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageSubresource.mipLevel = 0;
		imageSubresource.arrayLayer = 0;

		VkSubresourceLayout subresourceLayout;
		vkGetImageSubresourceLayout(logicalDevice->GetLogicalDevice(), dstImage, &imageSubresource, &subresourceLayout);

		auto result = new uint8_t[subresourceLayout.size];

		void *data;
		vkMapMemory(logicalDevice->GetLogicalDevice(), dstImageMemory, subresourceLayout.offset, subresourceLayout.size, 0, &data);
		memcpy(result, data, static_cast<size_t>(subresourceLayout.size));
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), dstImageMemory);

		vkFreeMemory(logicalDevice->GetLogicalDevice(), dstImageMemory, nullptr);
		vkDestroyImage(logicalDevice->GetLogicalDevice(), dstImage, nullptr);

		return result;
	}

	uint8_t *Cubemap::GetPixels() const
	{
		auto result = (uint8_t *) malloc(m_width * m_height * 4 * 6);

		for (uint32_t i = 0; i < 6; i++)
		{
			auto sidePixels = GetPixels(i);
			memcpy(&result[m_width * m_height * 4 * i], sidePixels, m_width * m_height * 4);
		}

		return result;
	}

	void Cubemap::SetPixels(const uint8_t *pixels)
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		Buffer bufferStaging = Buffer(m_width * m_height * 4 * 6, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory(), 0, bufferStaging.GetSize(), 0, &data);
		memcpy(data, pixels, bufferStaging.GetSize());
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), bufferStaging.GetBufferMemory());
	}
}
