#include "VertexAnimatedData.hpp"

namespace acid
{
	const int32_t VertexAnimatedData::NO_INDEX = -1;

	VertexAnimatedData::VertexAnimatedData(const uint32_t &index, const Vector3 &position) :
		m_position(position),
		m_uvIndex(NO_INDEX),
		m_normalIndex(NO_INDEX),
		m_duplicateVertex(nullptr),
		m_index(index),
		m_skinData(nullptr)
	{
	}

	VertexAnimatedData::~VertexAnimatedData()
	{
	}

	bool VertexAnimatedData::IsSet() const
	{
		return m_uvIndex != NO_INDEX && m_normalIndex != NO_INDEX;
	}

	bool VertexAnimatedData::HasSameTextureAndNormal(const int &textureIndexOther, const int &normalIndexOther) const
	{
		return textureIndexOther == m_uvIndex && normalIndexOther == m_normalIndex;
	}
}
