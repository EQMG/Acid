#include "Cubemap.hpp"

#include <cmath>

namespace fl
{
	const std::vector<std::string> Cubemap::SIDE_FILE_SUFFIXS = {"Right", "Left", "Top", "Bottom", "Back", "Front"};

	Cubemap::Cubemap(const std::string &filename, const std::string &fileExt, const bool &mipmap) :
		IResource(),
		Buffer(LoadSize(filename, fileExt), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		IDescriptor(),
		m_filename(filename),
		m_fileExt(fileExt),
		m_mipLevels(1),
		m_components(0),
		m_width(0),
		m_height(0),
		m_depth(0),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_format(VK_FORMAT_R8G8B8A8_UNORM),
		m_imageInfo({})
	{
#if FL_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif

		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		stbi_uc *pixels = LoadPixels(filename, fileExt, m_size, &m_width, &m_height, &m_depth, &m_components);

		m_mipLevels = mipmap ? Texture::GetMipLevels(m_width, m_height, m_depth) : 1;

		Buffer *bufferStaging = new Buffer(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void *data;
		vkMapMemory(logicalDevice, bufferStaging->GetBufferMemory(), 0, m_size, 0, &data);
		memcpy(data, pixels, m_size);
		vkUnmapMemory(logicalDevice, bufferStaging->GetBufferMemory());

		Texture::CreateImage(m_width, m_height, m_depth, m_mipLevels, m_format, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image, m_bufferMemory, 6);
		Texture::TransitionImageLayout(m_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, 6);
		Texture::CopyBufferToImage(m_width, m_height, m_depth, bufferStaging->GetBuffer(), m_image, 6);
		Texture::CreateMipmaps(m_image, m_width, m_height, m_depth, m_mipLevels, 6);
		Texture::TransitionImageLayout(m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_mipLevels, 6);
		Texture::CreateImageSampler(true, false, m_mipLevels, m_sampler);
		Texture::CreateImageView(m_image, VK_IMAGE_VIEW_TYPE_CUBE, m_format, m_mipLevels, m_imageView, 6);

		Buffer::CopyBuffer(bufferStaging->GetBuffer(), m_buffer, m_size);

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_imageInfo.imageView = m_imageView;
		m_imageInfo.sampler = m_sampler;

		delete bufferStaging;
		stbi_image_free(pixels);

#if FL_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();
		printf("Cubemap '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	Cubemap::~Cubemap()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		vkDestroySampler(logicalDevice, m_sampler, nullptr);
		vkDestroyImage(logicalDevice, m_image, nullptr);
		vkDestroyImageView(logicalDevice, m_imageView, nullptr);
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
		descriptorPoolSize.descriptorCount = 1;

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

	VkDeviceSize Cubemap::LoadSize(const std::string &filename, const std::string &fileExt)
	{
		VkDeviceSize size = 0;

		for (const auto suffix : SIDE_FILE_SUFFIXS)
		{
			const std::string filepathSide = filename + "/" + suffix + fileExt;
			const VkDeviceSize sizeSide = Texture::LoadSize(filepathSide);
			size += sizeSide;
		}

		return size;
	}

	stbi_uc *Cubemap::LoadPixels(const std::string &filename, const std::string &fileExt, const size_t &bufferSize, int *width, int *height, int *depth, int *components)
	{
		stbi_uc *pixels = (stbi_uc *) malloc(bufferSize);
		stbi_uc *offset = pixels;

		for (const auto suffix : SIDE_FILE_SUFFIXS)
		{
			std::string filepathSide = filename + "/" + suffix + fileExt;
			VkDeviceSize sizeSide = Texture::LoadSize(filepathSide);
			stbi_uc *pixelsSide = Texture::LoadPixels(filepathSide, width, height, components);
			depth = width;

			memcpy(offset, pixelsSide, sizeSide);
			offset += sizeSide;
			free(pixelsSide);
		}

		return pixels;
	}
}
