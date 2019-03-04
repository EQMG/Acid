#include "VertexModel.hpp"

namespace acid
{
	VertexModel::VertexModel(const Vector3 &position, const Vector2 &uv, const Vector3 &normal) :
		m_position(position),
		m_uv(uv),
		m_normal(normal)
	{
	}

	Shader::VertexInput VertexModel::GetVertexInput(const uint32_t &binding)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The vertex input description.
		bindingDescriptions[0].binding = binding;
		bindingDescriptions[0].stride = sizeof(VertexModel);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

		// Position attribute.
		attributeDescriptions[0].binding = binding;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(VertexModel, m_position);

		// UV attribute.
		attributeDescriptions[1].binding = binding;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(VertexModel, m_uv);

		// Normal attribute.
		attributeDescriptions[2].binding = binding;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(VertexModel, m_normal);

		return Shader::VertexInput(binding, bindingDescriptions, attributeDescriptions);
	}
}
