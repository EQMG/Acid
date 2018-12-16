#include "GeometryLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid
{
	GeometryLoader::GeometryLoader(Metadata *libraryGeometries, const std::vector<VertexWeights> &vertexWeights) :
		m_meshData(libraryGeometries->FindChild("geometry")->FindChild("mesh")),
		m_vertexWeights(vertexWeights),
		m_positionsList(std::vector<std::unique_ptr<VertexAnimatedData>>()),
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

		for (auto &current : m_positionsList)
		{
			Vector3 position = current->GetPosition();
			Vector2 textures = m_uvsList.at(current->GetUvIndex());
			Vector3 normal = m_normalsList.at(current->GetNormalIndex());
			Vector3 tangent = Vector3::ZERO;

			auto skinData = current->GetSkinData();
			Vector3 jointIds = Vector3(skinData.GetJointIds()[0], skinData.GetJointIds()[1], skinData.GetJointIds()[2]);
			Vector3 weights = Vector3(skinData.GetWeights()[0], skinData.GetWeights()[1], skinData.GetWeights()[2]);

			m_vertices.emplace_back(VertexAnimated(position, textures, normal, tangent, jointIds, weights));
		}
	}

	void GeometryLoader::LoadVertices()
	{
		std::string positionsSource = m_meshData->FindChild("vertices")->FindChild("input")->FindAttribute("source").substr(1);
		auto positionsData = m_meshData->FindChildWithAttribute("source", "id", positionsSource)->FindChild("float_array");
		uint32_t positionsCount = String::From<uint32_t>(positionsData->FindAttribute("count"));
		auto positionsRawData = String::Split(positionsData->GetValue(), " ");

		for (uint32_t i = 0; i < positionsCount / 3; i++)
		{
			Vector4 position = Vector4(String::From<float>(positionsRawData[i * 3]), String::From<float>(positionsRawData[i * 3 + 1]), String::From<float>(positionsRawData[i * 3 + 2]), 1.0f);
			position = MeshAnimated::CORRECTION.Transform(position);
			VertexAnimatedData *newVertex = new VertexAnimatedData(static_cast<int32_t>(m_positionsList.size()), position);
			newVertex->SetSkinData(m_vertexWeights[m_positionsList.size()]);
			m_positionsList.emplace_back(newVertex);
		}
	}

	void GeometryLoader::LoadUvs()
	{
		std::string uvsSource = m_meshData->FindChildWithBackup("polylist", "triangles")->FindChildWithAttribute("input", "semantic", "TEXCOORD")->FindAttribute("source").substr(1);
		auto uvsData = m_meshData->FindChildWithAttribute("source", "id", uvsSource)->FindChild("float_array");
		uint32_t uvsCount = String::From<uint32_t>(uvsData->FindAttribute("count"));
		auto uvsRawData = String::Split(uvsData->GetValue(), " ");

		for (uint32_t i = 0; i < uvsCount / 2; i++)
		{
			Vector2 uv = Vector2(String::From<float>(uvsRawData[i * 2]), 1.0f - String::From<float>(uvsRawData[i * 2 + 1]));
			m_uvsList.emplace_back(uv);
		}
	}

	void GeometryLoader::LoadNormals()
	{
		std::string normalsSource = m_meshData->FindChildWithBackup("polylist", "triangles")->FindChildWithAttribute("input", "semantic", "NORMAL")->FindAttribute("source").substr(1);
		auto normalsData = m_meshData->FindChildWithAttribute("source", "id", normalsSource)->FindChild("float_array");
		uint32_t normalsCount = String::From<uint32_t>(normalsData->FindAttribute("count"));
		auto normalsRawData = String::Split(normalsData->GetValue(), " ");

		for (uint32_t i = 0; i < normalsCount / 3; i++)
		{
			Vector4 normal = Vector4(String::From<float>(normalsRawData[i * 3]), String::From<float>(normalsRawData[i * 3 + 1]), String::From<float>(normalsRawData[i * 3 + 2]), 0.0f);
			normal = MeshAnimated::CORRECTION.Transform(normal);
			m_normalsList.emplace_back(normal);
		}
	}

	void GeometryLoader::AssembleVertices()
	{
		int32_t indexCount = static_cast<int32_t>(m_meshData->FindChildWithBackup("polylist", "triangles")->FindChildren("input").size());
		auto indexRawData = String::Split(m_meshData->FindChildWithBackup("polylist", "triangles")->FindChild("p")->GetValue(), " ");

		for (uint32_t i = 0; i < indexRawData.size() / indexCount; i++)
		{
			int32_t positionIndex = String::From<int32_t>(indexRawData[i * indexCount]);
			int32_t normalIndex = String::From<int32_t>(indexRawData[i * indexCount + 1]);
			int32_t uvIndex = String::From<int32_t>(indexRawData[i * indexCount + 2]);
			ProcessVertex(positionIndex, normalIndex, uvIndex);
		}
	}

	VertexAnimatedData *GeometryLoader::ProcessVertex(const int32_t &positionIndex, const int32_t &normalIndex, const int32_t &uvIndex)
	{
		auto currentVertex = m_positionsList[positionIndex].get();

		if (!currentVertex->IsSet())
		{
			currentVertex->SetUvIndex(uvIndex);
			currentVertex->SetNormalIndex(normalIndex);
			m_indices.emplace_back(positionIndex);
			return currentVertex;
		}
		else
		{
			return DealWithAlreadyProcessedVertex(currentVertex, uvIndex, normalIndex);
		}
	}

	VertexAnimatedData *GeometryLoader::DealWithAlreadyProcessedVertex(VertexAnimatedData *previousVertex, const int32_t &newUvIndex, const int32_t &newNormalIndex)
	{
		if (previousVertex->HasSameTextureAndNormal(newUvIndex, newNormalIndex))
		{
			m_indices.emplace_back(previousVertex->GetIndex());
			return previousVertex;
		}
		else
		{
			VertexAnimatedData *anotherVertex = nullptr;

			for (auto &position : m_positionsList)
			{
				if (position.get() == previousVertex->GetDuplicateVertex())
				{
					anotherVertex = position.get();
					break;
				}
			}

			if (anotherVertex != nullptr)
			{
				return DealWithAlreadyProcessedVertex(anotherVertex, newUvIndex, newNormalIndex);
			}
			else
			{
				auto duplicateVertex = new VertexAnimatedData(static_cast<uint32_t>(m_positionsList.size()), previousVertex->GetPosition());
				duplicateVertex->SetUvIndex(newUvIndex);
				duplicateVertex->SetNormalIndex(newNormalIndex);
				duplicateVertex->SetSkinData(previousVertex->GetSkinData());
				previousVertex->SetDuplicateVertex(duplicateVertex);
				m_positionsList.emplace_back(duplicateVertex);
				m_indices.emplace_back(duplicateVertex->GetIndex());
				return duplicateVertex;
			}
		}
	}

	void GeometryLoader::RemoveUnusedVertices()
	{
		for (auto &vertex : m_positionsList)
		{
			if (!vertex->IsSet())
			{
				vertex->SetUvIndex(0);
				vertex->SetNormalIndex(0);
			}
		}
	}
}
