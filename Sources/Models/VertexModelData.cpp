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

	bool VertexModelData::IsSet() const
	{
		return m_uvIndex.has_value() && m_normalIndex.has_value();
	}

	bool VertexModelData::HasSameUvAndNormal(const std::optional<uint32_t> &uvIndexOther, const std::optional<uint32_t> &normalIndexOther) const
	{
		return uvIndexOther == m_uvIndex && normalIndexOther == m_normalIndex;
	}
}
