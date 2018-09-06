#pragma once

#include <cstdint>
#include "Animations/Skin/VertexSkinData.hpp"
#include "Maths/Vector3.hpp"

namespace acid
{
	class ACID_EXPORT VertexAnimatedData
	{
	private:
		static const int32_t NO_INDEX;

		Vector3 m_position;

		int32_t m_uvIndex;
		int32_t m_normalIndex;
		VertexAnimatedData *m_duplicateVertex;

		int32_t m_index;

		VertexSkinData m_skinData;
	public:
		VertexAnimatedData(const int32_t &index, const Vector3 &position);

		Vector3 GetPosition() const { return m_position; }

		int32_t GetUvIndex() const { return m_uvIndex; }

		void SetUvIndex(const int32_t &uvIndex) { m_uvIndex = uvIndex; }

		int32_t GetNormalIndex() const { return m_normalIndex; }

		void SetNormalIndex(const int32_t &normalIndex) { m_normalIndex = normalIndex; }

		VertexAnimatedData *GetDuplicateVertex() const { return m_duplicateVertex; }

		void SetDuplicateVertex(VertexAnimatedData *duplicateVertex) { m_duplicateVertex = duplicateVertex; }

		int32_t GetIndex() const { return m_index; }

		bool IsSet() const;

		bool HasSameTextureAndNormal(const int32_t &textureIndexOther, const int32_t &normalIndexOther) const;

		VertexSkinData GetSkinData() const { return m_skinData; }

		void SetSkinData(const VertexSkinData &skinData) { m_skinData = skinData; }
	};
}
