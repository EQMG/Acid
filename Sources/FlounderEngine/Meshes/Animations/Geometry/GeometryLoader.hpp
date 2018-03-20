#pragma once

#include "../../../Maths/Vector3.hpp"
#include "../../../Models/Model.hpp"
#include "../Skin/SkinLoader.hpp"
#include "VertexAnimated.hpp"

namespace Flounder
{
	class F_EXPORT GeometryData
	{
	private:
		std::vector<VertexModel> m_vertices;
		std::vector<uint32_t> m_indices;

	public:
		GeometryData(const std::vector<VertexModel> &vertices, const std::vector<uint32_t> &indices) :
			m_vertices(vertices),
			m_indices(indices)
		{
		}

		~GeometryData()
		{
		}

		std::vector<VertexModel> GetVertices() const { return m_vertices; }

		std::vector<uint32_t> GetIndices() const { return m_indices; }
	};

	class F_EXPORT GeometryLoader
	{
	private:
		LoadedValue *m_meshData;

		std::vector<VertexSkinData*> m_vertexWeights;

		std::vector<std::pair<VertexData *, VertexSkinData*>> m_positionsList;
		std::vector<Vector2> m_uvsList;
		std::vector<Vector3> m_normalsList;

		std::vector<VertexModel> m_verticesList;
		std::vector<uint32_t> m_indicesList;

		GeometryData *m_geometryData;
	public:
		GeometryLoader(LoadedValue *libraryGeometries, const std::vector<VertexSkinData*> &vertexWeights);

		~GeometryLoader();

		GeometryData *GetData() const { return m_geometryData; }

	private:
		void LoadVertices();

		void LoadUvs();

		void LoadNormals();

		void AssembleVertices();

		std::pair<VertexData *, VertexSkinData*> ProcessVertex(const int &positionIndex, const int &normalIndex, const int &uvIndex);

		std::pair<VertexData *, VertexSkinData*> DealWithAlreadyProcessedVertex(const std::pair<VertexData *, VertexSkinData*> &previousVertex, const int &newUvIndex, const int &newNormalIndex);

		void RemoveUnusedVertices();
	};
}
