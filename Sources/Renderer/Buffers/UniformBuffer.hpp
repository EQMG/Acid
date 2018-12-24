#pragma once

#include <vulkan/vulkan.h>
#include "Renderer/Descriptors/IDescriptor.hpp"
#include "Renderer/Pipelines/ShaderProgram.hpp"
#include "Buffer.hpp"

namespace acid
{
	class ACID_EXPORT UniformBuffer :
		public IDescriptor,
		public Buffer
	{
	public:
		explicit UniformBuffer(const VkDeviceSize &size);

		void Update(const void *newData);

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage, const uint32_t &count);

		WriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType,
			const DescriptorSet &descriptorSet, const std::optional<OffsetSize> &offsetSize) const override;
	};
}
