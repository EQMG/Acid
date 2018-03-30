#include "VertexModel.hpp"

namespace Flounder
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
		void *data = new void*[dataSize];

		std::vector<VertexModel> thisVector = std::vector<VertexModel>();

		for (auto vertex : vertices)
		{
			thisVector.push_back(*((VertexModel*) vertex));
		}

		memcpy(data, thisVector.data(), dataSize);
		return data;
	}

	std::vector<VkVertexInputBindingDescription> VertexModel::GetInputDescription()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The vertex input description.
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(VertexModel);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}
}
