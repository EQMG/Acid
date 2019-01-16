#include "VertexModelData.hpp"

namespace acid
{
	static const int32_t NO_INDEX = -1;

	VertexModelData::VertexModelData(const uint32_t &index, const Vector3 &position) :
		m_position(position),
		m_uvIndex(NO_INDEX),
		m_normalIndex(NO_INDEX),
		m_duplicateVertex(nullptr),
		m_index(index),
		m_length(position.Length()),
		m_tangents(std::vector<Vector3>()),
		m_averagedTangent(Vector3())
	{
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

		if (m_averagedTangent.Length() != 0.0f)
		{
			m_averagedTangent = m_averagedTangent.Normalize();
		}
	}

	bool VertexModelData::IsSet() const
	{
		return m_uvIndex != NO_INDEX && m_normalIndex != NO_INDEX;
	}

	bool VertexModelData::HasSameTextureAndNormal(const int32_t &textureIndexOther, const int32_t &normalIndexOther) const
	{
		return textureIndexOther == m_uvIndex && normalIndexOther == m_normalIndex;
	}
}
