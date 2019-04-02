#include "GeometryLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid
{
GeometryLoader::GeometryLoader(const Metadata *libraryGeometries, std::vector<VertexWeights> vertexWeights, const Matrix4 &correction) :
	m_meshData(libraryGeometries->FindChild("geometry")->FindChild("mesh")),
	m_vertexWeights(std::move(vertexWeights)),
	m_correction(correction)
{
	auto positions = GetPositions();
	auto uvs = GetUvs();
	auto normals = GetNormals();

	auto indexCount = static_cast<int32_t>(m_meshData->FindChildWithBackup("polylist", "triangles")->FindChildren("input").size());
	auto indexRawData = String::Split(m_meshData->FindChildWithBackup("polylist", "triangles")->FindChild("p")->GetValue(), " ");

	std::unordered_map<VertexAnimated, size_t> uniqueVertices;

	for (uint32_t i = 0; i < indexRawData.size() / indexCount; i++)
	{
		auto positionIndex = String::From<uint32_t>(indexRawData[i * indexCount]);
		auto normalIndex = String::From<uint32_t>(indexRawData[i * indexCount + 1]);
		auto uvIndex = String::From<uint32_t>(indexRawData[i * indexCount + 2]);

		auto vertexWeight = m_vertexWeights[positionIndex];
		Vector3 jointIds = Vector3(vertexWeight.GetJointIds()[0], vertexWeight.GetJointIds()[1], vertexWeight.GetJointIds()[2]);
		Vector3 weights = Vector3(vertexWeight.GetWeights()[0], vertexWeight.GetWeights()[1], vertexWeight.GetWeights()[2]);

		VertexAnimated vertex = VertexAnimated(positions[positionIndex], uvs[uvIndex], normals[normalIndex], jointIds, weights);

		if (uniqueVertices.count(vertex) == 0)
		{
			uniqueVertices[vertex] = m_vertices.size();
			m_vertices.emplace_back(vertex);
		}

		m_indices.emplace_back(static_cast<uint32_t>(uniqueVertices[vertex]));
	}
}

std::vector<Vector3> GeometryLoader::GetPositions()
{
	std::string positionsSource = m_meshData->FindChild("vertices")->FindChild("input")->FindAttribute("source").substr(1);
	auto positionsData = m_meshData->FindChildWithAttribute("source", "id", positionsSource)->FindChild("float_array");
	auto positionsCount = String::From<uint32_t>(positionsData->FindAttribute("count"));
	auto positionsRawData = String::Split(positionsData->GetValue(), " ");

	std::vector<Vector3> positions;

	for (uint32_t i = 0; i < positionsCount / 3; i++)
	{
		Vector4 position = Vector4(String::From<float>(positionsRawData[i * 3]), String::From<float>(positionsRawData[i * 3 + 1]), String::From<float>(positionsRawData[i * 3 + 2]));
		positions.emplace_back(m_correction.Transform(position));
	}

	return positions;
}

std::vector<Vector2> GeometryLoader::GetUvs()
{
	std::string uvsSource = m_meshData->FindChildWithBackup("polylist", "triangles")->FindChildWithAttribute("input", "semantic", "TEXCOORD")->FindAttribute("source").substr(1);
	auto uvsData = m_meshData->FindChildWithAttribute("source", "id", uvsSource)->FindChild("float_array");
	auto uvsCount = String::From<uint32_t>(uvsData->FindAttribute("count"));
	auto uvsRawData = String::Split(uvsData->GetValue(), " ");

	std::vector<Vector2> uvs;

	for (uint32_t i = 0; i < uvsCount / 2; i++)
	{
		Vector2 uv = Vector2(String::From<float>(uvsRawData[i * 2]), 1.0f - String::From<float>(uvsRawData[i * 2 + 1]));
		uvs.emplace_back(uv);
	}

	return uvs;
}

std::vector<Vector3> GeometryLoader::GetNormals()
{
	std::string normalsSource = m_meshData->FindChildWithBackup("polylist", "triangles")->FindChildWithAttribute("input", "semantic", "NORMAL")->FindAttribute("source").substr(1);
	auto normalsData = m_meshData->FindChildWithAttribute("source", "id", normalsSource)->FindChild("float_array");
	auto normalsCount = String::From<uint32_t>(normalsData->FindAttribute("count"));
	auto normalsRawData = String::Split(normalsData->GetValue(), " ");

	std::vector<Vector3> normals;

	for (uint32_t i = 0; i < normalsCount / 3; i++)
	{
		Vector4 normal = Vector4(String::From<float>(normalsRawData[i * 3]), String::From<float>(normalsRawData[i * 3 + 1]), String::From<float>(normalsRawData[i * 3 + 2]));
		normals.emplace_back(m_correction.Transform(normal));
	}

	return normals;
}
}
