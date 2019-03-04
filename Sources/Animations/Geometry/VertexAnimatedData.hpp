#pragma once

#include <cstdint>
#include <optional>
#include "Animations/Skin/VertexWeights.hpp"
#include "Maths/Vector3.hpp"

namespace acid
{
	class ACID_EXPORT VertexAnimatedData
	{
	public:
		VertexAnimatedData(const int32_t &index, const Vector3 &position);

		const uint32_t &GetIndex() const { return m_index; }

		const std::optional<uint32_t> &GetUvIndex() const { return m_uvIndex; }

		void SetUvIndex(const std::optional<uint32_t> &uvIndex) { m_uvIndex = uvIndex; }

		const std::optional<uint32_t> &GetNormalIndex() const { return m_normalIndex; }

		void SetNormalIndex(const std::optional<uint32_t> &normalIndex) { m_normalIndex = normalIndex; }

		VertexAnimatedData *GetDuplicateVertex() const { return m_duplicateVertex; }

		void SetDuplicateVertex(VertexAnimatedData *duplicateVertex) { m_duplicateVertex = duplicateVertex; }

		const Vector3 &GetPosition() const { return m_position; }

		const VertexWeights &GetSkinData() const { return m_skinData; }

		void SetSkinData(const VertexWeights &skinData) { m_skinData = skinData; }

		bool IsSet() const;

		bool HasSameUvAndNormal(const std::optional<uint32_t> &uvIndexOther, const std::optional<uint32_t> &normalIndexOther) const;
	private:
		uint32_t m_index;
		std::optional<uint32_t> m_uvIndex;
		std::optional<uint32_t> m_normalIndex;
		VertexAnimatedData *m_duplicateVertex;
		Vector3 m_position;
		VertexWeights m_skinData;
	};
}
