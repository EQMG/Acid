#pragma once

#include "Maths/Vector3.hpp"
#include "Helpers/NonCopyable.hpp"
#include "Models/Model.hpp"
#include "VertexAnimated.hpp"
#include "VertexAnimatedData.hpp"

namespace acid
{
	class ACID_EXPORT GeometryLoader :
		public NonCopyable
	{
	private:
		Metadata *m_meshData;

		std::vector<VertexWeights> m_vertexWeights;

		std::vector<std::unique_ptr<VertexAnimatedData>> m_positionsList;
		std::vector<Vector2> m_uvsList;
		std::vector<Vector3> m_normalsList;

		std::vector<VertexAnimated> m_vertices;
		std::vector<uint32_t> m_indices;
	public:
		GeometryLoader(Metadata *libraryGeometries, const std::vector<VertexWeights> &vertexWeights);

		const std::vector<VertexAnimated> &GetVertices() const { return m_vertices; }

		const std::vector<uint32_t> &GetIndices() const { return m_indices; }
	private:
		void LoadVertices();

		void LoadUvs();

		void LoadNormals();

		void AssembleVertices();

		VertexAnimatedData *ProcessVertex(const int32_t &positionIndex, const int32_t &normalIndex, const int32_t &uvIndex);

		VertexAnimatedData *DealWithAlreadyProcessedVertex(VertexAnimatedData *previousVertex, const int32_t &newUvIndex, const int32_t &newNormalIndex);

		void RemoveUnusedVertices();
	};
}
