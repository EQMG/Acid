#include "Cubemap.hpp"

#include <cmath>
#include "Display/Display.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
	const std::vector<std::string> Cubemap::SIDE_FILE_SUFFIXES = {"Right", "Left", "Top", "Bottom", "Back", "Front"};

	std::shared_ptr<Cubemap> Cubemap::Resource(const std::string &filename, const std::string &fileExt)
	{
		std::string suffixToken = "/" + SIDE_FILE_SUFFIXES[0] + fileExt;
		std::string realFilename = Files::SearchFile(filename + suffixToken);
		realFilename = String::Replace(realFilename, suffixToken, "");
		auto resource = Resources::Get()->Get(realFilename);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<Cubemap>(resource);
		}

		auto result = std::make_shared<Cubemap>(realFilename, fileExt);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	Cubemap::Cubemap(const std::string &filename, const std::string &fileExt, const bool &repeatEdges, const bool &mipmap, const bool &anisotropic, const bool &nearest) :
		IResource(),
		Buffer(Texture::LoadSize(filename, fileExt, SIDE_FILE_SUFFIXES), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		IDescriptor(),
		m_filename(filename),
		m_fileExt(fileExt),
		m_repeatEdges(repeatEdges),
		m_mipLevels(1),
		m_anisotropic(anisotropic),
		m_nearest(nearest),
		m_components(0),
		m_width(0),
		m_height(0),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM),
		m_imageInfo({})
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		auto logicalDevice = Display::Get()->GetLogicalDevice();

		auto pixels = Texture::LoadPixels(filename, fileExt, SIDE_FILE_SUFFIXES, m_size, &m_width, &m_height, &m_components);

		m_mipLevels = mipmap ? Texture::GetMipLevels(m_width, m_height) : 1;

		Buffer *bufferStaging = new Buffer(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice, bufferStaging->GetBufferMemory(), 0, m_size, 0, &data);
		memcpy(data, pixels, m_size);
		vkUnmapMemory(logicalDevice, bufferStaging->GetBufferMemory());

		Texture::CreateImage(m_image, m_bufferMemory, m_width, m_height, VK_IMAGE_TYPE_2D, VK_SAMPLE_COUNT_1_BIT, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 6);
		Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels, 6);
		Texture::CopyBufferToImage(bufferStaging->GetBuffer(), m_image, m_width, m_height, 6);

		if (mipmap)
		{
			Texture::CreateMipmaps(m_image, m_width, m_height, m_mipLevels, 6);
		}
		else
		{
			Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_mipLevels, 6);
		}

		Texture::CreateImageSampler(m_sampler, m_repeatEdges, m_anisotropic, m_nearest, m_mipLevels);
		Texture::CreateImageView(m_image, m_imageView,VK_IMAGE_VIEW_TYPE_CUBE, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels,  6);

		Buffer::CopyBuffer(bufferStaging->GetBuffer(), m_buffer, m_size);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;

		delete bufferStaging;
		Texture::DeletePixels(pixels);

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		Log::Out("Cubemap '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	Cubemap::Cubemap(const uint32_t &width, const uint32_t &height, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage, const VkSampleCountFlagBits &samples) :
		IResource(),
		Buffer(width * height * 4 * 6, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		IDescriptor(),
		m_filename(""),
		m_fileExt(""),
		m_repeatEdges(true),
		m_mipLevels(1),
		m_anisotropic(false),
		m_nearest(false),
		m_components(4),
		m_width(width),
		m_height(height),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM),
		m_imageInfo({})
	{
		Texture::CreateImage(m_image, m_bufferMemory, m_width, m_height, VK_IMAGE_TYPE_2D, samples, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL,
			usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 6);
		Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels, 6);
		Texture::CreateImageSampler(m_sampler, m_repeatEdges, m_anisotropic, m_nearest, m_mipLevels);
		Texture::CreateImageView(m_image, m_imageView, VK_IMAGE_VIEW_TYPE_CUBE, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 6);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;
	}

	Cubemap::Cubemap(const uint32_t &width, const uint32_t &height, float *pixels, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage, const VkSampleCountFlagBits &samples) :
		IResource(),
		Buffer(width * height * 4 * 6, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		IDescriptor(),
		m_filename(""),
		m_fileExt(""),
		m_repeatEdges(true),
		m_mipLevels(1),
		m_anisotropic(false),
		m_nearest(false),
		m_components(4),
		m_width(width),
		m_height(height),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM),
		m_imageInfo({})
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		Buffer *bufferStaging = new Buffer(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice, bufferStaging->GetBufferMemory(), 0, m_size, 0, &data);
		memcpy(data, pixels, m_size);
		vkUnmapMemory(logicalDevice, bufferStaging->GetBufferMemory());

		Texture::CreateImage(m_image, m_bufferMemory, m_width, m_height, VK_IMAGE_TYPE_2D, samples, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL,
			usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 6);
		Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels, 6);
		Texture::CopyBufferToImage(bufferStaging->GetBuffer(), m_image, m_width, m_height, 6);
	//	Texture::CreateMipmaps(m_image, m_width, m_height, m_mipLevels, 6);
		Texture::TransitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_mipLevels, 6);
		Texture::CreateImageSampler(m_sampler, m_repeatEdges, m_anisotropic, m_nearest, m_mipLevels);
		Texture::CreateImageView(m_image, m_imageView, VK_IMAGE_VIEW_TYPE_CUBE, m_format, m_mipLevels, VK_IMAGE_ASPECT_COLOR_BIT, 6);

		Buffer::CopyBuffer(bufferStaging->GetBuffer(), m_buffer, m_size);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;

		delete bufferStaging;
		delete[] pixels;
	}

	Cubemap::~Cubemap()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroySampler(logicalDevice, m_sampler, nullptr);
		vkDestroyImageView(logicalDevice, m_imageView, nullptr);
		vkDestroyImage(logicalDevice, m_image, nullptr);
	}

	DescriptorType Cubemap::CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		descriptorSetLayoutBinding.stageFlags = stage;

		VkDescriptorPoolSize descriptorPoolSize = {};
		descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorPoolSize.descriptorCount = 64; // Arbitrary number.

		return DescriptorType(binding, stage, descriptorSetLayoutBinding, descriptorPoolSize);
	}

	VkWriteDescriptorSet Cubemap::GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const
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
