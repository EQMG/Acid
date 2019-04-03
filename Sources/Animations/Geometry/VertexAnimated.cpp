#include "VertexAnimated.hpp"

namespace acid
{
VertexAnimated::VertexAnimated(const Vector3f &position, const Vector2f &uv, const Vector3f &normal, const Vector3ui &jointId, const Vector3f &vertexWeight) :
	m_position(position),
	m_uv(uv),
	m_normal(normal),
	m_jointId(jointId),
	m_vertexWeight(vertexWeight)
{
}

bool VertexAnimated::operator==(const VertexAnimated &other) const
{
	return m_position == other.m_position && m_uv == other.m_uv && m_normal == other.m_normal && m_jointId == other.m_jointId && m_vertexWeight == other.m_vertexWeight;
}

bool VertexAnimated::operator!=(const VertexAnimated &other) const
{
	return !(*this == other);
}

Shader::VertexInput VertexAnimated::GetVertexInput(const uint32_t &binding)
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

	// The vertex input description.
	bindingDescriptions[0].binding = binding;
	bindingDescriptions[0].stride = sizeof(VertexAnimated);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(5);

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

	// Joint Id attribute.
	attributeDescriptions[3].binding = binding;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SINT;
	attributeDescriptions[3].offset = offsetof(VertexAnimated, m_jointId);

	// Vertex Weight attribute.
	attributeDescriptions[4].binding = binding;
	attributeDescriptions[4].location = 4;
	attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[4].offset = offsetof(VertexAnimated, m_vertexWeight);

	return Shader::VertexInput(binding, bindingDescriptions, attributeDescriptions);
}
}
