#include "Cubemap.hpp"

#include <cmath>
#include "Display/Display.hpp"
#include "Helpers/String.hpp"
#include "Resources/Resources.hpp"
#include "Renderer/Buffers/Buffer.hpp"
#include "Texture.hpp"

namespace acid
{
	const std::vector<std::string> Cubemap::FILE_SIDES = {"Right", "Left", "Top", "Bottom", "Back", "Front"};

	std::shared_ptr<Cubemap> Cubemap::Resource(const std::string &filename, const std::string &fileExt)
	{
		if (filename.empty())
		{
			return nullptr;
		}

		auto resource = Resources::Get()->Find(filename);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<Cubemap>(resource);
		}

		auto result = std::make_shared<Cubemap>(filename, fileExt);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	Cubemap::Cubemap(const std::string &filename, const std::string &fileSuffix, const VkFilter &filter, const VkSamplerAddressMode &addressMode,
					 const bool &anisotropic, const bool &mipmap) :
		IResource(),
		IDescriptor(),
		m_filename(filename),
		m_fileSuffix(fileSuffix),
		m_filter(filter),
		m_addressMode(addressMode),
		m_anisotropic(anisotropic),
		m_mipLevels(1),
		m_samples(VK_SAMPLE_COUNT_1_BIT),
		m_imageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
		m_components(0),
		m_width(0),
		m_height(0),
		m_image(VK_NULL_HANDLE),
		m_deviceMemory(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM),
		m_imageInfo({})
	{
#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		auto logicalDevice = Display::Get()->GetLogicalDevice();

		auto pixels = Texture::LoadPixels(m_filename, m_fileSuffix, FILE_SIDES, &m_width, &m_height, &m_components);

		m_mipLevels = mipmap ? Texture::GetMipLevels(m_width, m_height) : 1;

		Buffer bufferStaging = Buffer(m_width * m_height * 4 * 6, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice, bufferStaging.GetBufferMemory(), 0, bufferStaging.GetSize(), 0, &data);
		memcpy(data, pixels, bufferStaging.GetSize());
		vkUnmapMemory(logicalDevice, bufferStaging.GetBufferMemory());

		Texture::CreateImage(m_image, m_deviceMemory, m_width, m_height, VK_IMAGE_TYPE_2D, m_samples, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 6);
		Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels, 0, 6);
		Texture::CopyBufferToImage(bufferStaging.GetBuffer(), m_image, m_width, m_height, 0, 6);

		if (mipmap)
		{
			Texture::CreateMipmaps(m_image, m_width, m_height, m_imageLayout, m_mipLevels, 0, 6);
		}
		else
		{
			Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_imageLayout, m_mipLevels, 0, 6);
		}

		Texture::CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);
		Texture::CreateImageView(m_image, m_imageView,VK_IMAGE_VIEW_TYPE_CUBE, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 6);

		m_imageInfo.imageLayout = m_imageLayout;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;

		Texture::DeletePixels(pixels);

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("Cubemap '%s' loaded in %ims\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif
	}

	Cubemap::Cubemap(const uint32_t &width, const uint32_t &height, void *pixels, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage,
					 const VkFilter &filter, const VkSamplerAddressMode &addressMode, const VkSampleCountFlagBits &samples, const bool &anisotropic, const bool &mipmap) :
		IResource(),
		IDescriptor(),
		m_filename(""),
		m_fileSuffix(""),
		m_filter(filter),
		m_addressMode(addressMode),
		m_anisotropic(anisotropic),
		m_mipLevels(1),
		m_samples(samples),
		m_imageLayout(imageLayout),
		m_components(4),
		m_width(width),
		m_height(height),
		m_image(VK_NULL_HANDLE),
		m_deviceMemory(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM),
		m_imageInfo({})
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		m_mipLevels = mipmap ? Texture::GetMipLevels(m_width, m_height) : 1;

		Texture::CreateImage(m_image, m_deviceMemory, m_width, m_height, VK_IMAGE_TYPE_2D, m_samples, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL,
			usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 6);

		if (pixels != nullptr || mipmap)
		{
			Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels, 0, 6);
		}

		if (pixels != nullptr)
		{
			Buffer bufferStaging = Buffer(width * height * 4 * 6, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			void *data;
			Display::CheckVk(vkMapMemory(logicalDevice, bufferStaging.GetBufferMemory(), 0, bufferStaging.GetSize(), 0, &data));
			memcpy(data, pixels, bufferStaging.GetSize());
			vkUnmapMemory(logicalDevice, bufferStaging.GetBufferMemory());

			Texture::CopyBufferToImage(bufferStaging.GetBuffer(), m_image, m_width, m_height, 0, 6);
		}

		if (mipmap)
		{
			Texture::CreateMipmaps(m_image, m_width, m_height, m_imageLayout, m_mipLevels, 0, 6);
		}
		else if (pixels != nullptr)
		{
			Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_imageLayout, m_mipLevels, 0, 6);
		}
		else
		{
			Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, m_imageLayout, m_mipLevels, 0, 6);
		}

		Texture::CreateImageSampler(m_sampler, m_filter, m_addressMode, m_anisotropic, m_mipLevels);
		Texture::CreateImageView(m_image, m_imageView, VK_IMAGE_VIEW_TYPE_CUBE, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 6);

		m_imageInfo.imageLayout = m_imageLayout;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;
	}

	Cubemap::~Cubemap()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroySampler(logicalDevice, m_sampler, nullptr);
		vkDestroyImageView(logicalDevice, m_imageView, nullptr);
		vkFreeMemory(logicalDevice, m_deviceMemory, nullptr);
		vkDestroyImage(logicalDevice, m_image, nullptr);
	}

	DescriptorType Cubemap::CreateDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage, const uint32_t &count)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		descriptorSetLayoutBinding.stageFlags = stage;

		VkDescriptorPoolSize descriptorPoolSize = {};
		descriptorPoolSize.type = descriptorType;
		descriptorPoolSize.descriptorCount = count;

		return DescriptorType(binding, stage, descriptorSetLayoutBinding, descriptorPoolSize);
	}

	VkWriteDescriptorSet Cubemap::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const DescriptorSet &descriptorSet) const
	{
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet.GetDescriptorSet();
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = descriptorType;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &m_imageInfo;

		return descriptorWrite;
	}

	uint8_t *Cubemap::GetPixels(const uint32_t &arrayLayer)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkImage dstImage;
		VkDeviceMemory dstImageMemory;
		Texture::CopyImage(m_image, dstImage, dstImageMemory, m_width, m_height, false, arrayLayer, 6);

		VkImageSubresource imageSubresource = {};
		imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageSubresource.mipLevel = 0;
		imageSubresource.arrayLayer = 0;

		VkSubresourceLayout subresourceLayout;
		vkGetImageSubresourceLayout(logicalDevice, dstImage, &imageSubresource, &subresourceLayout);

		uint8_t *result = new uint8_t[subresourceLayout.size];

		void *data;
		vkMapMemory(logicalDevice, dstImageMemory, subresourceLayout.offset, subresourceLayout.size, 0, &data);
		memcpy(result, data, static_cast<size_t>(subresourceLayout.size));
		vkUnmapMemory(logicalDevice, dstImageMemory);

		vkFreeMemory(logicalDevice, dstImageMemory, nullptr);
		vkDestroyImage(logicalDevice, dstImage, nullptr);

		return result;
	}

	uint8_t *Cubemap::GetPixels()
	{
		auto result = (uint8_t *) malloc(m_width * m_height * 4 * 6);

		for (uint32_t i = 0; i < 6; i++)
		{
			auto sidePixels = GetPixels(i);
			memcpy(&result[m_width * m_height * 4 * i], sidePixels, m_width * m_height * 4);
		}

		return result;
	}

	void Cubemap::SetPixels(uint8_t *pixels)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		Buffer bufferStaging = Buffer(m_width * m_height * 4 * 6, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice, bufferStaging.GetBufferMemory(), 0, bufferStaging.GetSize(), 0, &data);
		memcpy(data, pixels, bufferStaging.GetSize());
		vkUnmapMemory(logicalDevice, bufferStaging.GetBufferMemory());
	}
}
