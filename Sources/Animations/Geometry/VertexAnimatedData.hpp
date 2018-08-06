#pragma once

#include <cstdint>
#include <vector>
#include "Animations/Skin/VertexSkinData.hpp"
#include "Maths/Vector3.hpp"

namespace acid
{
	class ACID_EXPORT VertexAnimatedData
	{
	private:
		static const uint32_t NO_INDEX;

		Vector3 m_position;

		int m_uvIndex;
		int m_normalIndex;
		VertexAnimatedData *m_duplicateVertex;

		uint32_t m_index;
		float m_length;

		std::vector<Vector3 *> m_tangents;
		Vector3 m_averagedTangent;

		VertexSkinData *m_skinData;
	public:
		VertexAnimatedData(const uint32_t &index, const Vector3 &position);

		~VertexAnimatedData();

		Vector3 GetPosition() const { return m_position; }

		int GetUvIndex() const { return m_uvIndex; }

		void SetUvIndex(const int &uvIndex) { m_uvIndex = uvIndex; }

		int GetNormalIndex() const { return m_normalIndex; }

		void SetNormalIndex(const int &normalIndex) { m_normalIndex = normalIndex; }

		VertexAnimatedData *GetDuplicateVertex() const { return m_duplicateVertex; }

		void SetDuplicateVertex(VertexAnimatedData *duplicateVertex) { m_duplicateVertex = duplicateVertex; }

		uint32_t GetIndex() const { return m_index; }

		float GetLength() const { return m_length; }

		void AddTangent(Vector3 *tangent);

		void AverageTangents();

		Vector3 GetAverageTangent() const { return m_averagedTangent; }

		bool IsSet() const;

		bool HasSameTextureAndNormal(const int &textureIndexOther, const int &normalIndexOther) const;

		VertexSkinData *GetSkinData() const { return m_skinData; }

		void SetSkinData(VertexSkinData *skinData) { m_skinData = skinData; }
	};
}
