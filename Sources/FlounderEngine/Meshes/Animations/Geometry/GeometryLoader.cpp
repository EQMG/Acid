#include "GeometryLoader.hpp"

#include "../MeshAnimated.hpp"

namespace Flounder
{
	GeometryLoader::GeometryLoader(LoadedValue *libraryGeometries, const std::vector<VertexSkinData *> &vertexWeights) :
		m_meshData(libraryGeometries->GetChild("geometry")->GetChild("mesh")),
		m_vertexWeights(vertexWeights),
		m_positionsList(std::vector<std::pair<VertexData *, VertexSkinData *>>()),
		m_uvsList(std::vector<Vector2>()),
		m_normalsList(std::vector<Vector3>()),
		m_vertices(std::vector<VertexAnimated>()),
		m_indices(std::vector<uint32_t>())
	{
		LoadVertices();
		LoadUvs();
		LoadNormals();
		AssembleVertices();
		RemoveUnusedVertices();

		for (auto current : m_positionsList)
		{
			const Vector3 position = current.first->GetPosition();
			const Vector2 textures = m_uvsList.at(current.first->GetUvIndex());
			const Vector3 normal = m_normalsList.at(current.first->GetNormalIndex());
			const Vector3 tangent = current.first->GetAverageTangent();

			//	const VertexSkinData* skin = current.second;
			//	const Vector3 jointIds = Vector3(skin->GetJointIds()[0], skin->GetJointIds()[1], skin->GetJointIds()[2]);
			//	const Vector3 weights = Vector3(skin->GetWeights()[0], skin->GetWeights()[1], skin->GetWeights()[2]);

			VertexAnimated vertex = VertexAnimated(position, textures, normal, tangent); // , jointIds, weights

			m_vertices.push_back(vertex);

			delete current.first;
		}
	}

	GeometryLoader::~GeometryLoader()
	{
	}

	void GeometryLoader::LoadVertices()
	{
		std::string positionsSource = m_meshData->GetChild("vertices")->GetChild("input")->GetChild("-source")->GetString().substr(1);
		LoadedValue *positionsData = m_meshData->GetChildWithAttribute("source", "-id", positionsSource)->GetChild("float_array");
		int positionsCount = stoi(positionsData->GetChild("-count")->GetString());
		auto positionsRawData = FormatString::Split(positionsData->GetChild("#text")->GetString(), " ");

		for (int i = 0; i < positionsCount / 3; i++)
		{
			Vector4 position = Vector4(stof(positionsRawData[i * 3]), stof(positionsRawData[i * 3 + 1]), stof(positionsRawData[i * 3 + 2]), 1.0f);
			Matrix4::Transform(*MeshAnimated::S_CORRECTION, position, &position); // Matrix4::Multiply
			VertexData *newVertex = new VertexData(m_positionsList.size(), position);
			m_positionsList.push_back(std::make_pair(newVertex, m_vertexWeights.at(m_vertices.size())));
		}
	}

	void GeometryLoader::LoadUvs()
	{
		std::string uvsSource = m_meshData->GetChild("polylist")->GetChildWithAttribute("input", "-semantic", "TEXCOORD")->GetChild("-source")->GetString().substr(1);
		LoadedValue *uvsData = m_meshData->GetChildWithAttribute("source", "-id", uvsSource)->GetChild("float_array");
		int uvsCount = stoi(uvsData->GetChild("-count")->GetString());
		auto uvsRawData = FormatString::Split(uvsData->GetChild("#text")->GetString(), " ");

		for (int i = 0; i < uvsCount / 2; i++)
		{
			Vector2 uv = Vector2(stof(uvsRawData[i * 2]), 1.0f - stof(uvsRawData[i * 2 + 1]));
			m_uvsList.push_back(uv);
		}
	}

	void GeometryLoader::LoadNormals()
	{
		std::string normalsSource = m_meshData->GetChild("polylist")->GetChildWithAttribute("input", "-semantic", "NORMAL")->GetChild("-source")->GetString().substr(1);
		LoadedValue *normalsData = m_meshData->GetChildWithAttribute("source", "-id", normalsSource)->GetChild("float_array");
		int normalsCount = stoi(normalsData->GetChild("-count")->GetString());
		auto normalsRawData = FormatString::Split(normalsData->GetChild("#text")->GetString(), " ");

		for (int i = 0; i < normalsCount / 3; i++)
		{
			Vector3 normal = Vector3(stof(normalsRawData[i * 3]), stof(normalsRawData[i * 3 + 1]), stof(normalsRawData[i * 3 + 2]));
			m_normalsList.push_back(normal);
		}
	}

	void GeometryLoader::AssembleVertices()
	{
		int indexCount = m_meshData->GetChild("polylist")->GetChild("input")->m_children.size();
		auto indexRawData = FormatString::Split(m_meshData->GetChild("polylist")->GetChild("p")->GetString(), " ");

		for (int i = 0; i < indexRawData.size() / indexCount; i++)
		{
			int positionIndex = stoi(indexRawData[i * indexCount]);
			int normalIndex = stoi(indexRawData[i * indexCount + 1]);
			int uvIndex = stoi(indexRawData[i * indexCount + 2]);
			ProcessVertex(positionIndex, normalIndex, uvIndex);
		}
	}

	std::pair<VertexData *, VertexSkinData *> GeometryLoader::ProcessVertex(const int &positionIndex, const int &normalIndex, const int &uvIndex)
	{
		auto currentVertex = m_positionsList.at(positionIndex);

		if (!currentVertex.first->IsSet())
		{
			currentVertex.first->SetUvIndex(uvIndex);
			currentVertex.first->SetNormalIndex(normalIndex);
			m_indices.push_back(positionIndex);
			return currentVertex;
		}
		else
		{
			return DealWithAlreadyProcessedVertex(currentVertex, uvIndex, normalIndex);
		}
	}

	std::pair<VertexData *, VertexSkinData *> GeometryLoader::DealWithAlreadyProcessedVertex(const std::pair<VertexData *, VertexSkinData *> &previousVertex, const int &newUvIndex, const int &newNormalIndex)
	{
		if (previousVertex.first->HasSameTextureAndNormal(newUvIndex, newNormalIndex))
		{
			m_indices.push_back(previousVertex.first->GetIndex());
			return previousVertex;
		}
		else
		{
			std::pair<VertexData *, VertexSkinData *> anotherVertex;

			for (auto position : m_positionsList)
			{
				if (position.first == previousVertex.first->GetDuplicateVertex())
				{
					anotherVertex = position;
					break;
				}
			}

			if (anotherVertex.first != nullptr)
			{
				return DealWithAlreadyProcessedVertex(anotherVertex, newUvIndex, newNormalIndex);
			}
			else
			{
				auto duplicateVertex = std::make_pair(new VertexData(m_positionsList.size(), previousVertex.first->GetPosition()), previousVertex.second);
				duplicateVertex.first->SetUvIndex(newUvIndex);
				duplicateVertex.first->SetNormalIndex(newNormalIndex);
				previousVertex.first->SetDuplicateVertex(duplicateVertex.first);
				m_positionsList.push_back(duplicateVertex);
				m_indices.push_back(duplicateVertex.first->GetIndex());
				return duplicateVertex;
			}
		}
	}

	void GeometryLoader::RemoveUnusedVertices()
	{
		for (auto vertex : m_positionsList)
		{
			vertex.first->AverageTangents();

			if (!vertex.first->IsSet())
			{
				vertex.first->SetUvIndex(0);
				vertex.first->SetNormalIndex(0);
			}
		}
	}
}
