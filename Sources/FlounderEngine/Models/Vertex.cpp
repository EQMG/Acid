#include "Vertex.hpp"

namespace Flounder
{
	Vertex::Vertex(const Vector3 &position, const Vector2 &uv, const Vector3 &normal, const Vector3 &tangent) :
		m_position(position),
		m_uv(uv),
		m_normal(normal),
		m_tangent(tangent)
	{
	}

	Vertex::~Vertex()
	{
	}

	std::vector<VkVertexInputBindingDescription> Vertex::GetBindingDescriptions(const VkVertexInputRate &inputRate)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The vertex input description.
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = inputRate;

		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> Vertex::GetAttributeDescriptions(const int &usedCount)
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(usedCount + 1);

		// Position attribute.
		if (usedCount >= 0)
		{
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, m_position);
		}

		// UV attribute.
		if (usedCount >= 1)
		{
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, m_uv);
		}

		// Normal attribute.
		if (usedCount >= 2)
		{
			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, m_normal);
		}

		// Tangent attribute.
		if (usedCount >= 3)
		{
			attributeDescriptions[3].binding = 0;
			attributeDescriptions[3].location = 3;
			attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[3].offset = offsetof(Vertex, m_tangent);
		}

		return attributeDescriptions;
	}
}
