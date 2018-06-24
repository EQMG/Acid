#pragma once

#include "ShaderProgram.hpp"

namespace fl
{
	class FL_EXPORT IPipeline
	{
	public:
		IPipeline()
		{
		}

		virtual ~IPipeline()
		{
		}

		virtual ShaderProgram *GetShaderProgram() const = 0;

		virtual VkDescriptorSetLayout GetVkDescriptorSetLayout() const = 0;

		virtual VkDescriptorPool GetVkDescriptorPool() const = 0;

		virtual VkPipeline GetVkPipeline() const = 0;

		virtual VkPipelineLayout GetVkPipelineLayout() const = 0;
	};
}
