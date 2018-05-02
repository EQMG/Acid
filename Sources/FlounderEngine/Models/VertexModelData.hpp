#pragma once

#include <vector>
#include <cstdint>
#include "Maths/Vector3.hpp"

namespace fl
{
	class FL_EXPORT VertexModelData
	{
	private:
		static const uint32_t NO_INDEX;

		Vector3 m_position;

		int m_uvIndex;
		int m_normalIndex;
		VertexModelData *m_duplicateVertex;

		uint32_t m_index;
		float m_length;

		std::vector<Vector3 *> m_tangents;
		Vector3 m_averagedTangent;
	public:
		VertexModelData(const uint32_t &index, const Vector3 &position);

		~VertexModelData();

		Vector3 GetPosition() const { return m_position; }

		int GetUvIndex() const { return m_uvIndex; }

		void SetUvIndex(const int &uvIndex) { m_uvIndex = uvIndex; }

		int GetNormalIndex() const { return m_normalIndex; }

		void SetNormalIndex(const int &normalIndex) { m_normalIndex = normalIndex; }

		VertexModelData *GetDuplicateVertex() const { return m_duplicateVertex; }

		void SetDuplicateVertex(VertexModelData *duplicateVertex) { m_duplicateVertex = duplicateVertex; }

		uint32_t GetIndex() const { return m_index; }

		float GetLength() const { return m_length; }

		void AddTangent(Vector3 *tangent);

		void AverageTangents();

		Vector3 GetAverageTangent() const { return m_averagedTangent; }

		bool IsSet() const;

		bool HasSameTextureAndNormal(const int &textureIndexOther, const int &normalIndexOther) const;
	};
}
