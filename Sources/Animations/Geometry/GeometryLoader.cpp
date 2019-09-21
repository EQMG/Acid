#include "GeometryLoader.hpp"

#include "Animations/MeshAnimated.hpp"

namespace acid {
GeometryLoader::GeometryLoader(NodeReturn libraryGeometries, std::vector<VertexWeights> vertexWeights, const Matrix4 &correction) :
	m_meshData(libraryGeometries["geometry"]["mesh"]),
	m_vertexWeights(std::move(vertexWeights)),
	m_correction(correction) {
	auto positions = GetPositions();
	auto uvs = GetUvs();
	auto normals = GetNormals();

	auto indexCount = static_cast<int32_t>(m_meshData->GetPropertyWithBackup("polylist", "triangles")["input"]->GetProperties().size());
	auto indexRawData = String::Split(m_meshData->GetPropertyWithBackup("polylist", "triangles")["p"]->GetValue(), ' ');

	std::unordered_map<VertexAnimated, size_t> uniqueVertices;

	for (uint32_t i = 0; i < indexRawData.size() / indexCount; i++) {
		auto positionIndex = String::From<uint32_t>(indexRawData[indexCount * i]);
		auto normalIndex = String::From<uint32_t>(indexRawData[indexCount * i + 1]);
		auto uvIndex = String::From<uint32_t>(indexRawData[indexCount * i + 2]);

		auto vertexWeight = m_vertexWeights[positionIndex];
		Vector3ui jointIds(vertexWeight.GetJointIds()[0], vertexWeight.GetJointIds()[1], vertexWeight.GetJointIds()[2]);
		Vector3f weights(vertexWeight.GetWeights()[0], vertexWeight.GetWeights()[1], vertexWeight.GetWeights()[2]);

		VertexAnimated vertex(positions[positionIndex], uvs[uvIndex], normals[normalIndex], jointIds, weights);

		if (uniqueVertices.count(vertex) == 0) {
			uniqueVertices[vertex] = m_vertices.size();
			m_vertices.emplace_back(vertex);
		}

		m_indices.emplace_back(static_cast<uint32_t>(uniqueVertices[vertex]));
	}
}

std::vector<Vector3f> GeometryLoader::GetPositions() const {
	auto positionsSource = m_meshData["vertices"]["input"]["-source"]->GetValue().substr(1);
	auto positionsData = m_meshData["source"]->GetPropertyWithValue("-id", positionsSource)["float_array"];
	auto positionsCount = positionsData["-count"]->Get<uint32_t>();
	auto positionsRawData = String::Split(positionsData["#text"]->GetValue(), ' ');

	std::vector<Vector3f> positions;

	for (uint32_t i = 0; i < positionsCount / 3; i++) {
		Vector4f position(String::From<float>(positionsRawData[3 * i]), String::From<float>(positionsRawData[i * 3 + 1]),
			String::From<float>(positionsRawData[3 * i + 2]));
		positions.emplace_back(m_correction.Transform(position));
	}

	return positions;
}

std::vector<Vector2f> GeometryLoader::GetUvs() const {
	auto uvsSource = m_meshData->GetPropertyWithBackup("polylist", "triangles")["input"]->GetPropertyWithValue("-semantic", "TEXCOORD")["-source"]->GetValue().substr(1);
	auto uvsData = m_meshData["source"]->GetPropertyWithValue("-id", uvsSource)["float_array"];
	auto uvsCount = uvsData["-count"]->Get<uint32_t>();
	auto uvsRawData = String::Split(uvsData["#text"]->GetValue(), ' ');

	std::vector<Vector2f> uvs;

	for (uint32_t i = 0; i < uvsCount / 2; i++) {
		Vector2f uv(String::From<float>(uvsRawData[2 * i]), 1.0f - String::From<float>(uvsRawData[2 * i + 1]));
		uvs.emplace_back(uv);
	}

	return uvs;
}

std::vector<Vector3f> GeometryLoader::GetNormals() const {
	auto normalsSource = m_meshData->GetPropertyWithBackup("polylist", "triangles")["input"]->GetPropertyWithValue("-semantic", "NORMAL")["-source"]->GetValue().substr(1);
	auto normalsData = m_meshData["source"]->GetPropertyWithValue("-id", normalsSource)["float_array"];
	auto normalsCount = normalsData["-count"]->Get<uint32_t>();
	auto normalsRawData = String::Split(normalsData["#text"]->GetValue(), ' ');

	std::vector<Vector3f> normals;

	for (uint32_t i = 0; i < normalsCount / 3; i++) {
		Vector4f normal(String::From<float>(normalsRawData[3 * i]), String::From<float>(normalsRawData[3 * i + 1]), String::From<float>(normalsRawData[3 * i + 2]));
		normals.emplace_back(m_correction.Transform(normal));
	}

	return normals;
}
}
