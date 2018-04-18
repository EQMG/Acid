#pragma once

#include <map>
#include "Renderer/Pipelines/Descriptor.hpp"
#include "Renderer/Pipelines/PipelineCreate.hpp"
#include "Renderer/Pipelines/ShaderProgram.hpp"
#include "Buffer.hpp"

namespace Flounder
{
	class F_EXPORT UniformBuffer :
		public Buffer,
		public Descriptor
	{
	private:
		VkDescriptorBufferInfo m_bufferInfo;
	public:
		UniformBuffer(const VkDeviceSize &size);

		~UniformBuffer();

		void Update(void *newData);

		/*template<typename ... T>
		void Update(const std::string &blockName, ShaderProgram *shaderProgram, std::pair<std::string, T> values)
		{
			auto uniformBlock = shaderProgram->GetUniformBlock(blockName);
			void *newData = malloc(m_size);

			for (auto pair : newDataMap)
			{
				auto uniform = uniformBlock->GetUniform(pair.first);

				if (uniform == nullptr)
				{
					continue;
				}

				memcpy(newData + uniform->m_offset, pair.second, uniform->m_size);
			}

			Update(newData);
			free(newData);
		}*/

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;
	};
}
