#pragma once

#include <array>

#include "../platforms/glfw/GlfwVulkan.hpp"

#include "../maths/Vector2.hpp"
#include "../maths/Vector3.hpp"
#include "../renderer/buffers/BaseVertex.hpp"

namespace Flounder
{
	class Vertex :
		public BaseVertex
	{
	public:
		Vector3 position;
		Vector2 textures;
		Vector3 normal;
		Vector3 tangent;

		Vertex();

		~Vertex();

		VkVertexInputBindingDescription GetBindingDescription() override;

		std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() override;
	};
}
