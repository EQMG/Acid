#include "VertexAnimated.hpp"

namespace Flounder
{
	VertexAnimated::VertexAnimated(const Vector3 &position, const Vector2 &uv, const Vector3 &normal, const Vector3 &tangent, const Vector3 &jointId, const Vector3 &vertexWeight) :
		IVertex(),
		m_position(position),
		m_uv(uv),
		m_normal(normal),
		m_tangent(tangent),
		m_jointId(jointId),
		m_vertexWeight(vertexWeight)
	{
	}

	VertexAnimated::VertexAnimated(const VertexAnimated &source) :
		IVertex(),
		m_position(source.m_position),
		m_uv(source.m_uv),
		m_normal(source.m_normal),
		m_tangent(source.m_tangent),
		m_jointId(source.m_jointId),
		m_vertexWeight(source.m_vertexWeight)
	{
	}

	VertexAnimated::~VertexAnimated()
	{
	}

	std::vector<VkVertexInputBindingDescription> VertexAnimated::GetBindingDescriptions(const VkVertexInputRate &inputRate)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The vertex input description.
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(VertexAnimated);
		bindingDescriptions[0].inputRate = inputRate;

		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> VertexAnimated::GetAttributeDescriptions(const int &usedCount)
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(usedCount + 1);

		// Position attribute.
		if (usedCount >= 0)
		{
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(VertexAnimated, m_position);
		}

		// UV attribute.
		if (usedCount >= 1)
		{
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(VertexAnimated, m_uv);
		}

		// Normal attribute.
		if (usedCount >= 2)
		{
			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(VertexAnimated, m_normal);
		}

		// Tangent attribute.
		if (usedCount >= 3)
		{
			attributeDescriptions[3].binding = 0;
			attributeDescriptions[3].location = 3;
			attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[3].offset = offsetof(VertexAnimated, m_tangent);
		}

		// Joint Id attribute.
		if (usedCount >= 4)
		{
			attributeDescriptions[4].binding = 0;
			attributeDescriptions[4].location = 3;
			attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[4].offset = offsetof(VertexAnimated, m_jointId);
		}

		// Vertex Weight attribute.
		if (usedCount >= 5)
		{
			attributeDescriptions[5].binding = 0;
			attributeDescriptions[5].location = 3;
			attributeDescriptions[5].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[5].offset = offsetof(VertexAnimated, m_vertexWeight);
		}

		return attributeDescriptions;
	}
}
