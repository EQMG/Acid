#pragma once

#include "Buffer.hpp"
#include "../pipelines/PipelineCreate.hpp"

namespace Flounder
{
	class UniformBuffer :
		public Buffer
	{
	public:
		UniformBuffer(const VkDeviceSize &size);

		~UniformBuffer();

		void Update(void *newData);

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorSet &descriptorSet) const;
	};
}
