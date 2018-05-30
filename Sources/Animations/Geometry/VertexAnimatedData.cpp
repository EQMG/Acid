#include "VertexAnimatedData.hpp"

namespace fl
{
	const uint32_t VertexAnimatedData::NO_INDEX = static_cast<uint32_t>(-1);

	VertexAnimatedData::VertexAnimatedData(const uint32_t &index, const Vector3 &position) :
		m_position(position),
		m_uvIndex(VertexAnimatedData::NO_INDEX),
		m_normalIndex(VertexAnimatedData::NO_INDEX),
		m_duplicateVertex(nullptr),
		m_index(index),
		m_length(position.Length()),
		m_tangents(std::vector<Vector3 *>()),
		m_averagedTangent(Vector3()),
		m_skinData(nullptr)
	{
	}

	VertexAnimatedData::~VertexAnimatedData()
	{
	}

	void VertexAnimatedData::AddTangent(Vector3 *tangent)
	{
		m_tangents.push_back(tangent);
	}

	void VertexAnimatedData::AverageTangents()
	{
		if (m_tangents.empty())
		{
			return;
		}

		for (auto it = m_tangents.begin(); it < m_tangents.end(); ++it)
		{
			m_averagedTangent += *(*it);
		}

		if (m_averagedTangent.Length() > 0.0f)
		{
			m_averagedTangent.Normalize();
		}
	}

	bool VertexAnimatedData::IsSet() const
	{
		return (static_cast<uint32_t>(m_uvIndex) != NO_INDEX) && (static_cast<uint32_t>(m_normalIndex) != NO_INDEX);
	}

	bool VertexAnimatedData::HasSameTextureAndNormal(const int &textureIndexOther, const int &normalIndexOther) const
	{
		return textureIndexOther == m_uvIndex && normalIndexOther == m_normalIndex;
	}
}
