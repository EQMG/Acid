#include "VertexAnimatedData.hpp"

namespace acid
{
	VertexAnimatedData::VertexAnimatedData(const int32_t &index, const Vector3 &position) :
		m_index(index),
		m_uvIndex({}),
		m_normalIndex({}),
		m_duplicateVertex(nullptr),
		m_position(position)
	{
	}

	bool VertexAnimatedData::IsSet() const
	{
		return m_uvIndex.has_value() && m_normalIndex.has_value();
	}

	bool VertexAnimatedData::HasSameTextureAndNormal(const int32_t &textureIndexOther, const int32_t &normalIndexOther) const
	{
		return textureIndexOther == m_uvIndex && normalIndexOther == m_normalIndex;
	}
}
