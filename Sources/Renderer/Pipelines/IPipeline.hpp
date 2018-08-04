#pragma once

#include <memory>
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

		virtual std::shared_ptr<ShaderProgram> GetShaderProgram() const = 0;

		virtual VkDescriptorSetLayout GetVkDescriptorSetLayout() const = 0;

		virtual VkDescriptorPool GetVkDescriptorPool() const = 0;

		virtual VkPipeline GetVkPipeline() const = 0;

		virtual VkPipelineLayout GetVkPipelineLayout() const = 0;
	};
}
