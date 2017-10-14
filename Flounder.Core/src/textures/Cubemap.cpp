#include "Cubemap.hpp"

namespace Flounder
{
	const std::vector<std::string> Cubemap::SIDE_FILE_SUFFIXS = { "Front", "Back", "Left", "Right", "Top", "Bottom" };

	Cubemap::Cubemap(const std::string &filename, const std::string &fileExt) :
		m_components(0),
		m_width(0),
		m_height(0),
		m_imageSize(0),
		m_image(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE),
		m_format(VK_FORMAT_UNDEFINED)
	{
		for (const auto suffix : SIDE_FILE_SUFFIXS)
		{
			const std::string filepathSide = filename + suffix + fileExt;
			const VkDeviceSize sizeSide = Texture::LoadSize(filepathSide);
			m_imageSize += sizeSide;
		}

		stbi_uc *pixels = (stbi_uc*) malloc(m_imageSize);
		stbi_uc *offset = pixels;

		/*for (const auto suffix : SIDE_FILE_SUFFIXS)
		{
			const std::string filepathSide = filename + suffix + fileExt;
			const VkDeviceSize sizeSide = Texture::LoadSize(filepathSide);
			const stbi_uc *pixelsSide = Texture::LoadPixels(filepathSide, &m_width, &m_height, &m_components);
			
			offset += sizeSide;
			memcpy(offset, pixelsSide, sizeSide);
		//	delete pixelsSide;
		}*/

		//delete pixels;
	}

	Cubemap::~Cubemap()
	{
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

	VkWriteDescriptorSet Cubemap::GetWriteDescriptor(const uint32_t &binding, const VkDescriptorSet &descriptorSet) const
	{
		// TODO: Don't create a descriptor like this!
		VkDescriptorImageInfo *descriptorInfo = new VkDescriptorImageInfo();
		descriptorInfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		descriptorInfo->imageView = m_imageView;
		descriptorInfo->sampler = m_sampler;

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = descriptorInfo;

		return descriptorWrite;
	}
}
