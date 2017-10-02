#pragma once

#include <array>

#include "../../platforms/glfw/GlfwVulkan.hpp"

#include "../../maths/Colour.hpp"
#include "../../maths/Vector3.hpp"

namespace Flounder
{
	struct Vertex
	{
		Vector3 position;
		Colour colour;
		Vector3 normal;

		static VkVertexInputBindingDescription GetBindingDescription();

		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
	};
}
