#include "GeometryLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid
{
GeometryLoader::GeometryLoader(const Node *libraryGeometries, std::vector<VertexWeights> vertexWeights, const Matrix4 &correction) :
	m_meshData{libraryGeometries->FindChild("geometry")->FindChild("mesh")},
	m_vertexWeights{std::move(vertexWeights)},
	m_correction{correction}
{
	auto positions = GetPositions();
	auto uvs = GetUvs();
	auto normals = GetNormals();

	auto indexCount = static_cast<int32_t>(m_meshData->FindChildWithBackup("polylist", "triangles")->FindChildren("input").size());
	auto indexRawData = String::Split(m_meshData->FindChildWithBackup("polylist", "triangles")->FindChild("p")->GetValue(), ' ');

	std::unordered_map<VertexAnimated, size_t> uniqueVertices;

	for (uint32_t i{}; i < indexRawData.size() / indexCount; i++)
	{
		auto positionIndex = String::From<uint32_t>(indexRawData[indexCount * i]);
		auto normalIndex = String::From<uint32_t>(indexRawData[indexCount * i + 1]);
		auto uvIndex = String::From<uint32_t>(indexRawData[indexCount * i + 2]);

		auto vertexWeight = m_vertexWeights[positionIndex];
		Vector3ui jointIds{vertexWeight.GetJointIds()[0], vertexWeight.GetJointIds()[1], vertexWeight.GetJointIds()[2]};
		Vector3f weights{vertexWeight.GetWeights()[0], vertexWeight.GetWeights()[1], vertexWeight.GetWeights()[2]};

		VertexAnimated vertex{positions[positionIndex], uvs[uvIndex], normals[normalIndex], jointIds, weights};

		if (uniqueVertices.count(vertex) == 0)
		{
			uniqueVertices[vertex] = m_vertices.size();
			m_vertices.emplace_back(vertex);
		}

		m_indices.emplace_back(static_cast<uint32_t>(uniqueVertices[vertex]));
	}
}

std::vector<Vector3f> GeometryLoader::GetPositions() const
{
	auto positionsSource = m_meshData->FindChild("vertices")->FindChild("input")->FindAttribute("source")->substr(1);
	auto positionsData = m_meshData->FindChildWithAttribute("source", "id", positionsSource)->FindChild("float_array");
	auto positionsCount = String::From<uint32_t>(*positionsData->FindAttribute("count"));
	auto positionsRawData = String::Split(positionsData->GetValue(), ' ');

	std::vector<Vector3f> positions;

	for (uint32_t i{}; i < positionsCount / 3; i++)
	{
		Vector4f position{String::From<float>(positionsRawData[3 * i]), String::From<float>(positionsRawData[i * 3 + 1]),
			String::From<float>(positionsRawData[3 * i + 2])};
		positions.emplace_back(m_correction.Transform(position));
	}

	return positions;
}

std::vector<Vector2f> GeometryLoader::GetUvs() const
{
	auto uvsSource = m_meshData->FindChildWithBackup("polylist", "triangles")->FindChildWithAttribute("input", "semantic", "TEXCOORD")->FindAttribute("source")->substr(1);
	auto uvsData = m_meshData->FindChildWithAttribute("source", "id", uvsSource)->FindChild("float_array");
	auto uvsCount = String::From<uint32_t>(*uvsData->FindAttribute("count"));
	auto uvsRawData = String::Split(uvsData->GetValue(), ' ');

	std::vector<Vector2f> uvs;

	for (uint32_t i{}; i < uvsCount / 2; i++)
	{
		Vector2f uv{String::From<float>(uvsRawData[2 * i]), 1.0f - String::From<float>(uvsRawData[2 * i + 1])};
		uvs.emplace_back(uv);
	}

	return uvs;
}

std::vector<Vector3f> GeometryLoader::GetNormals() const
{
	auto normalsSource = m_meshData->FindChildWithBackup("polylist", "triangles")->FindChildWithAttribute("input", "semantic", "NORMAL")->FindAttribute("source")->substr(1);
	auto normalsData = m_meshData->FindChildWithAttribute("source", "id", normalsSource)->FindChild("float_array");
	auto normalsCount = String::From<uint32_t>(*normalsData->FindAttribute("count"));
	auto normalsRawData = String::Split(normalsData->GetValue(), ' ');

	std::vector<Vector3f> normals;

	for (uint32_t i{}; i < normalsCount / 3; i++)
	{
		Vector4f normal{String::From<float>(normalsRawData[3 * i]), String::From<float>(normalsRawData[3 * i + 1]), String::From<float>(normalsRawData[3 * i + 2])};
		normals.emplace_back(m_correction.Transform(normal));
	}

	return normals;
}
}
