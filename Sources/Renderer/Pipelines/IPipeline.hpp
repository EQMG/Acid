#pragma once

#include "Renderer/Commands/CommandBuffer.hpp"
#include "ShaderProgram.hpp"

namespace acid
{
	class ACID_EXPORT IPipeline
	{
	public:
		void BindPipeline(const CommandBuffer &commandBuffer) const
		{
			vkCmdBindPipeline(commandBuffer.GetCommandBuffer(), GetPipelineBindPoint(), GetPipeline());
		}

		virtual ShaderProgram *GetShaderProgram() const = 0;

		virtual VkDescriptorSetLayout GetDescriptorSetLayout() const = 0;

		virtual VkDescriptorPool GetDescriptorPool() const = 0;

		virtual VkPipeline GetPipeline() const = 0;

		virtual VkPipelineLayout GetPipelineLayout() const = 0;

		virtual VkPipelineBindPoint GetPipelineBindPoint() const = 0;
	};
}
