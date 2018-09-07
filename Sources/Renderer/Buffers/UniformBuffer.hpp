#pragma once

#include <vulkan/vulkan.h>
#include "Renderer/Descriptors/IDescriptor.hpp"
#include "Renderer/Pipelines/ShaderProgram.hpp"
#include "Buffer.hpp"

namespace acid
{
	class ACID_EXPORT UniformBuffer :
		public Buffer,
		public IDescriptor
	{
	private:
		VkDescriptorBufferInfo m_bufferInfo;
	public:
		UniformBuffer(const VkDeviceSize &size);

		~UniformBuffer();

		void Update(const void *newData);

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;
	};
}
