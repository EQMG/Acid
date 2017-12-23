#include "VertexData.hpp"

namespace Flounder
{
	const uint32_t VertexData::NO_INDEX = -1;

	VertexData::VertexData(const uint32_t &index, const Vector3 &position) :
		m_position(position),
		m_uvIndex(VertexData::NO_INDEX),
		m_normalIndex(VertexData::NO_INDEX),
		m_duplicateVertex(nullptr),
		m_index(index),
		m_length(position.Length()),
		m_tangents(std::vector<Vector3*>()),
		m_averagedTangent(Vector3())
	{
	}

	VertexData::~VertexData()
	{
		//	delete m_duplicateVertex;
	}

	void VertexData::AddTangent(Vector3 *tangent)
	{
		m_tangents.push_back(tangent);
	}

	void VertexData::AverageTangents()
	{
		if (m_tangents.empty())
		{
			return;
		}

		for (auto it = m_tangents.begin(); it < m_tangents.end(); ++it)
		{
			Vector3::Add(m_averagedTangent, **it, &m_averagedTangent);
		}

		if (m_averagedTangent.Length() > 0.0f)
		{
			m_averagedTangent.Normalize();
		}
	}

	bool VertexData::IsSet() const
	{
		return (static_cast<uint32_t>(m_uvIndex) != NO_INDEX) && (static_cast<uint32_t>(m_normalIndex) != NO_INDEX);
	}

	bool VertexData::HasSameTextureAndNormal(const int &textureIndexOther, const int &normalIndexOther) const
	{
		return textureIndexOther == m_uvIndex && normalIndexOther == m_normalIndex;
	}
}
