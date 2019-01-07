#pragma once

#include <cstdint>
#include <vector>
#include "Maths/Vector3.hpp"

namespace acid
{
	class ACID_EXPORT VertexModelData
	{
	private:
		static const uint32_t NO_INDEX;

		Vector3 m_position;

		int32_t m_uvIndex;
		int32_t m_normalIndex;
		VertexModelData *m_duplicateVertex;

		uint32_t m_index;
		float m_length;

		std::vector<Vector3> m_tangents;
		Vector3 m_averagedTangent;
	public:
		VertexModelData(const uint32_t &index, const Vector3 &position);

		const Vector3 &GetPosition() const { return m_position; }

		const int32_t &GetUvIndex() const { return m_uvIndex; }

		void SetUvIndex(const int32_t &uvIndex) { m_uvIndex = uvIndex; }

		const int32_t &GetNormalIndex() const { return m_normalIndex; }

		void SetNormalIndex(const int32_t &normalIndex) { m_normalIndex = normalIndex; }

		VertexModelData *GetDuplicateVertex() const { return m_duplicateVertex; }

		void SetDuplicateVertex(VertexModelData *duplicateVertex) { m_duplicateVertex = duplicateVertex; }

		const uint32_t &GetIndex() const { return m_index; }

		const float &GetLength() const { return m_length; }

		void AddTangent(const Vector3 &tangent);

		void AverageTangents();

		const Vector3 &GetAverageTangent() const { return m_averagedTangent; }

		bool IsSet() const;

		bool HasSameTextureAndNormal(const int32_t &textureIndexOther, const int32_t &normalIndexOther) const;
	};
}
