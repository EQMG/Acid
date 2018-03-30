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

	void *VertexAnimated::GetData(std::vector<IVertex *> &vertices)
	{
		size_t dataSize = GetSize() * vertices.size();
		void *data = new void*[dataSize];

		std::vector<VertexAnimated> thisVector = std::vector<VertexAnimated>();

		for (auto vertex : vertices)
		{
			thisVector.push_back(*((VertexAnimated*) vertex));
		}

		memcpy(data, thisVector.data(), dataSize);
		return data;
	}

	std::vector<VkVertexInputBindingDescription> VertexAnimated::GetInputDescription()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The vertex input description.
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(VertexAnimated);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}
}
