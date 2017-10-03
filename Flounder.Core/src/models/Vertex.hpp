#pragma once

#include <vector>

#include "../platforms/glfw/GlfwVulkan.hpp"

#include "../maths/Vector2.hpp"
#include "../maths/Vector3.hpp"

namespace Flounder
{
	struct Vertex 
	{
	public:
		Vector3 position;
		Vector2 textures;
		Vector3 normal;
		Vector3 tangent;

		Vertex();

		~Vertex();

		static VkVertexInputBindingDescription GetBindingDescription();

		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	};
}
