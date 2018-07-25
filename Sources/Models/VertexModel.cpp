#include "VertexModel.hpp"

namespace acid
{
	VertexModel::VertexModel(const Vector3 &position, const Vector2 &uv, const Vector3 &normal, const Vector3 &tangent) :
		IVertex(),
		m_position(position),
		m_uv(uv),
		m_normal(normal),
		m_tangent(tangent)
	{
	}

	VertexModel::VertexModel(const VertexModel &source) :
		IVertex(),
		m_position(source.m_position),
		m_uv(source.m_uv),
		m_normal(source.m_normal),
		m_tangent(source.m_tangent)
	{
	}

	VertexModel::~VertexModel()
	{
	}

	void *VertexModel::GetData(std::vector<IVertex *> &vertices)
	{
		size_t dataSize = GetSize() * vertices.size();
		void *data = new void *[dataSize];

		std::vector<VertexModel> thisVector = std::vector<VertexModel>();

		for (auto &vertex : vertices)
		{
			thisVector.emplace_back(*((VertexModel *) vertex));
		}

		memcpy(data, thisVector.data(), dataSize);
		return data;
	}

	VertexInput VertexModel::GetVertexInput()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The vertex input description.
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(VertexModel);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(4);

		// Position attribute.
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(VertexModel, m_position);

		// UV attribute.
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(VertexModel, m_uv);

		// Normal attribute.
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(VertexModel, m_normal);

		// Tangent attribute.
		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(VertexModel, m_tangent);

		return VertexInput(bindingDescriptions, attributeDescriptions);
	}
}
