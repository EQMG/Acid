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
	private:
		VkDescriptorBufferInfo m_bufferInfo;
	public:
		explicit UniformBuffer(const VkDeviceSize &size);

		void Update(const void *newData);

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage, const uint32_t &count);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;
	};
}
