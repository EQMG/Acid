#pragma once

#include "../../../Maths/Vector3.hpp"
#include "../../../Models/Model.hpp"
#include "../Skin/SkinLoader.hpp"
#include "VertexAnimated.hpp"

namespace Flounder
{
	class F_EXPORT GeometryLoader
	{
	private:
		LoadedValue *m_meshData;

		std::vector<VertexSkinData *> m_vertexWeights;

		std::vector<std::pair<VertexModelData *, VertexSkinData *>> m_positionsList;
		std::vector<Vector2> m_uvsList;
		std::vector<Vector3> m_normalsList;

		std::vector<VertexAnimated> m_vertices;
		std::vector<uint32_t> m_indices;
	public:
		GeometryLoader(LoadedValue *libraryGeometries, const std::vector<VertexSkinData *> &vertexWeights);

		~GeometryLoader();

		std::vector<VertexAnimated> GetVertices() const { return m_vertices; }

		std::vector<uint32_t> GetIndices() const { return m_indices; }
	private:
		void LoadVertices();

		void LoadUvs();

		void LoadNormals();

		void AssembleVertices();

		std::pair<VertexModelData *, VertexSkinData *> ProcessVertex(const int &positionIndex, const int &normalIndex, const int &uvIndex);

		std::pair<VertexModelData *, VertexSkinData *> DealWithAlreadyProcessedVertex(const std::pair<VertexModelData *, VertexSkinData *> &previousVertex, const int &newUvIndex, const int &newNormalIndex);

		void RemoveUnusedVertices();
	};
}
