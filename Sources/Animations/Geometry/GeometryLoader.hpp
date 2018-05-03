#pragma once

#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "Animations/Skin/SkinLoader.hpp"
#include "VertexAnimated.hpp"
#include "VertexAnimatedData.hpp"

namespace fl
{
	class FL_EXPORT GeometryLoader
	{
	private:
		LoadedValue *m_meshData;

		std::vector<VertexSkinData *> m_vertexWeights;

		std::vector<VertexAnimatedData *> m_positionsList;
		std::vector<Vector2> m_uvsList;
		std::vector<Vector3> m_normalsList;

		std::vector<IVertex *> m_vertices;
		std::vector<uint32_t> m_indices;
	public:
		GeometryLoader(LoadedValue *libraryGeometries, const std::vector<VertexSkinData *> &vertexWeights);

		~GeometryLoader();

		std::vector<IVertex *> GetVertices() const { return m_vertices; }

		std::vector<uint32_t> GetIndices() const { return m_indices; }
	private:
		void LoadVertices();

		void LoadUvs();

		void LoadNormals();

		void AssembleVertices();

		VertexAnimatedData *ProcessVertex(const int &positionIndex, const int &normalIndex, const int &uvIndex);

		VertexAnimatedData *DealWithAlreadyProcessedVertex(VertexAnimatedData *previousVertex, const int &newUvIndex, const int &newNormalIndex);

		void RemoveUnusedVertices();
	};
}
