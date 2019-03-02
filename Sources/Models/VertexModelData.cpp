#include "VertexModelData.hpp"

namespace acid
{
	VertexModelData::VertexModelData(const uint32_t &index, const Vector3 &position) :
		m_index(index),
		m_positionIndex({}),
		m_uvIndex({}),
		m_normalIndex({}),
		m_duplicateVertex(nullptr),
		m_position(position)
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
		return m_uvIndex.has_value() && m_normalIndex.has_value();
	}

	bool VertexModelData::HasSameUvAndNormal(const std::optional<uint32_t> &uvIndexOther, const std::optional<uint32_t> &normalIndexOther) const
	{
		return uvIndexOther == m_uvIndex && normalIndexOther == m_normalIndex;
	}
}
