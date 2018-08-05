#pragma once

#include <memory>
#include "Display/Command/CommandBuffer.hpp"
#include "ShaderProgram.hpp"

namespace acid
{
	class ACID_EXPORT IPipeline
	{
	public:
		IPipeline()
		{
		}

		virtual ~IPipeline()
		{
		}

		void BindPipeline(const CommandBuffer &commandBuffer) const
		{
			vkCmdBindPipeline(commandBuffer.GetVkCommandBuffer(), GetVkPipelineBindPoint(), GetVkPipeline());
		}

		virtual std::shared_ptr<ShaderProgram> GetShaderProgram() const = 0;

		virtual VkDescriptorSetLayout GetVkDescriptorSetLayout() const = 0;

		virtual VkDescriptorPool GetVkDescriptorPool() const = 0;

		virtual VkPipeline GetVkPipeline() const = 0;

		virtual VkPipelineLayout GetVkPipelineLayout() const = 0;

		virtual VkPipelineBindPoint GetVkPipelineBindPoint() const = 0;
	};
}
