#pragma once

#include <array>

#include "../../platforms/glfw/GlfwVulkan.hpp"

#include "../../maths/Vector2.hpp"
#include "../../maths/Vector3.hpp"

namespace Flounder
{
	struct Vertex
	{
		Vector3 position;
		Vector2 textures;
		Vector3 normal;
		Vector3 tangent;

		static VkVertexInputBindingDescription GetBindingDescription();

		static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions();
	};
}
