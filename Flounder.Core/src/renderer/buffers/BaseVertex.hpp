#pragma once

#include <vector>
#include "../../platforms/glfw/GlfwVulkan.hpp"

namespace Flounder
{
	class BaseVertex
	{
	public:
		BaseVertex()
		{
		}

		virtual ~BaseVertex()
		{
		}

		virtual VkVertexInputBindingDescription GetBindingDescription() = 0;

		virtual std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() = 0;
	};
}
