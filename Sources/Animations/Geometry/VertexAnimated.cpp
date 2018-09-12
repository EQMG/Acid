#include "VertexAnimated.hpp"

namespace acid
{
	VertexAnimated::VertexAnimated(const Vector3 &position, const Vector2 &uv, const Vector3 &normal, const Vector3 &tangent, const Vector3 &jointId, const Vector3 &vertexWeight) :
		m_position(position),
		m_uv(uv),
		m_normal(normal),
		m_tangent(tangent),
		m_jointId(jointId),
		m_vertexWeight(vertexWeight)
	{
	}

	VertexInput VertexAnimated::GetVertexInput(const uint32_t &binding)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The vertex input description.
		bindingDescriptions[0].binding = binding;
		bindingDescriptions[0].stride = sizeof(VertexAnimated);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(6);

		// Position attribute.
		attributeDescriptions[0].binding = binding;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(VertexAnimated, m_position);

		// UV attribute.
		attributeDescriptions[1].binding = binding;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(VertexAnimated, m_uv);

		// Normal attribute.
		attributeDescriptions[2].binding = binding;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(VertexAnimated, m_normal);

		// Tangent attribute.
		attributeDescriptions[3].binding = binding;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(VertexAnimated, m_tangent);

		// Joint Id attribute.
		attributeDescriptions[4].binding = binding;
		attributeDescriptions[4].location = 4;
		attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SINT;
		attributeDescriptions[4].offset = offsetof(VertexAnimated, m_jointId);

		// Vertex Weight attribute.
		attributeDescriptions[5].binding = binding;
		attributeDescriptions[5].location = 5;
		attributeDescriptions[5].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[5].offset = offsetof(VertexAnimated, m_vertexWeight);

		return VertexInput(bindingDescriptions, attributeDescriptions);
	}
}
