#include "VertexAnimatedData.hpp"

namespace acid
{
	const int32_t VertexAnimatedData::NO_INDEX = -1;

	VertexAnimatedData::VertexAnimatedData(const int32_t &index, const Vector3 &position) :
		m_position(position),
		m_uvIndex(NO_INDEX),
		m_normalIndex(NO_INDEX),
		m_duplicateVertex(nullptr),
		m_index(index),
		m_skinData()
	{
	}

	VertexAnimatedData::~VertexAnimatedData()
	{
	}

	bool VertexAnimatedData::IsSet() const
	{
		return m_uvIndex != NO_INDEX && m_normalIndex != NO_INDEX;
	}

	bool VertexAnimatedData::HasSameTextureAndNormal(const int32_t &textureIndexOther, const int32_t &normalIndexOther) const
	{
		return textureIndexOther == m_uvIndex && normalIndexOther == m_normalIndex;
	}
}
