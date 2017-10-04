#pragma once

#include "../../platforms/glfw/Platform.hpp"

namespace Flounder
{
	struct InputState
	{
		uint32_t vertexBindingDescriptionCount;
		VkVertexInputBindingDescription *pVertexBindingDescriptions;

		uint32_t vertexAttributeDescriptionCount;
		VkVertexInputAttributeDescription *pVertexAttributeDescriptions;
	};
}