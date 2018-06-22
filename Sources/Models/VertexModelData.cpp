#include "VertexModelData.hpp"

namespace fl
{
	const uint32_t VertexModelData::NO_INDEX = static_cast<uint32_t>(-1);

	VertexModelData::VertexModelData(const uint32_t &index, const Vector3 &position) :
		m_position(position),
		m_uvIndex(VertexModelData::NO_INDEX),
		m_normalIndex(VertexModelData::NO_INDEX),
		m_duplicateVertex(nullptr),
		m_index(index),
		m_length(position.Length()),
		m_tangents(std::vector<Vector3>()),
		m_averagedTangent(Vector3())
	{
	}

	VertexModelData::~VertexModelData()
	{
		//	delete m_duplicateVertex;
	}

	void VertexModelData::AddTangent(const Vector3 &tangent)
	{
		m_tangents.emplace_back(tangent);
	}

	void VertexModelData::AverageTangents()
	{
		if (m_tangents.empty())
		{
			return;
		}

		for (auto it = m_tangents.begin(); it < m_tangents.end(); ++it)
		{
			m_averagedTangent += *it;
		}

		if (m_averagedTangent.Length() > 0.0f)
		{
			m_averagedTangent = m_averagedTangent.Normalize();
		}
	}

	bool VertexModelData::IsSet() const
	{
		return (static_cast<uint32_t>(m_uvIndex) != NO_INDEX) && (static_cast<uint32_t>(m_normalIndex) != NO_INDEX);
	}

	bool VertexModelData::HasSameTextureAndNormal(const int &textureIndexOther, const int &normalIndexOther) const
	{
		return textureIndexOther == m_uvIndex && normalIndexOther == m_normalIndex;
	}
}
