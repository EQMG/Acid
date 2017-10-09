#pragma once

#include "../../platforms/Platform.hpp"

namespace Flounder
{
	struct InputState
	{
		std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions;

		static InputState Create(const std::vector<VkVertexInputBindingDescription> &bindingDescription, const std::vector<VkVertexInputAttributeDescription> &attributeDescriptions)
		{
			InputState inputState = {};
			inputState.vertexBindingDescriptions = std::vector<VkVertexInputBindingDescription>(bindingDescription);
			inputState.vertexAttributeDescriptions = std::vector<VkVertexInputAttributeDescription>(attributeDescriptions);
			return inputState;
		}
	};
}
