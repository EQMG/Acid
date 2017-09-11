#pragma once

#include <array>

#include "../../platforms/glfw/GlfwVulkan.h"

//#include "../Maths/Colour.hpp"
//#include "../Maths/Vector2.hpp"
//#include "../Maths/Vector3.hpp"
#include "../../glm/glm.hpp"

namespace Flounder
{
	struct Vertex
	{
		glm::vec2 position;
		glm::vec3 colour;
	//	Vector2 position;
	//	Colour colour;

		static VkVertexInputBindingDescription getBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
			
			// Position attributes.
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, position);
			
			// Colour attributes.
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT; // VK_FORMAT_R32G32B32A32_SFLOAT
			attributeDescriptions[1].offset = offsetof(Vertex, colour);

			return attributeDescriptions;
		}
	};
}