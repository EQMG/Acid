#include "GeometryLoader.hpp"

#include "Animations/AnimatedMesh.hpp"

namespace acid {
GeometryLoader::GeometryLoader(NodeConstView &&libraryGeometries, std::vector<VertexWeights> vertexWeights, const Matrix4 &correction) :
	meshData(libraryGeometries["geometry"]["mesh"]),
	vertexWeights(std::move(vertexWeights)),
	correction(correction) {
	auto positions = GetPositions();
	auto uvs = GetUvs();
	auto normals = GetNormals();

	auto indexCount = static_cast<int32_t>(meshData.GetPropertyWithBackup("polylist", "triangles")["input"].GetProperties().size());
	auto indexRawData = String::Split(meshData.GetPropertyWithBackup("polylist", "triangles")["p"].Get<std::string>(), ' ');

	std::unordered_map<VertexAnimated, size_t> uniqueVertices;
	auto where = uniqueVertices.end();
	indices.reserve(indexRawData.size() / indexCount);

	for (uint32_t i = 0; i < indexRawData.size() / indexCount; i++) {
		auto positionIndex = String::From<uint32_t>(indexRawData[indexCount * i]);
		auto normalIndex = String::From<uint32_t>(indexRawData[indexCount * i + 1]);
		auto uvIndex = String::From<uint32_t>(indexRawData[indexCount * i + 2]);

		auto vertexWeight = this->vertexWeights[positionIndex];
		Vector3ui jointIds(vertexWeight.GetJointIds()[0], vertexWeight.GetJointIds()[1], vertexWeight.GetJointIds()[2]);
		Vector3f weights(vertexWeight.GetWeights()[0], vertexWeight.GetWeights()[1], vertexWeight.GetWeights()[2]);

		VertexAnimated vertex(positions[positionIndex], uvs[uvIndex], normals[normalIndex], jointIds, weights);

		if (uniqueVertices.count(vertex) == 0) {
			where = uniqueVertices.insert(where, {vertex, vertices.size()});
			vertices.emplace_back(vertex);
		}

		indices.emplace_back(static_cast<uint32_t>(uniqueVertices[vertex]));
	}
}

std::vector<Vector3f> GeometryLoader::GetPositions() const {
	auto positionsSource = meshData["vertices"]["input"]["-source"].Get<std::string>().substr(1);
	auto positionsData = meshData["source"].GetPropertyWithValue("-id", positionsSource)["float_array"];
	auto positionsCount = positionsData["-count"].Get<uint32_t>();
	auto positionsRawData = String::Split(positionsData.Get<std::string>(), ' ');

	std::vector<Vector3f> positions;
	positions.reserve(positionsCount / 3);

	for (uint32_t i = 0; i < positionsCount / 3; i++) {
		Vector4f position(String::From<float>(positionsRawData[3 * i]), String::From<float>(positionsRawData[i * 3 + 1]),
			String::From<float>(positionsRawData[3 * i + 2]));
		positions.emplace_back(correction.Transform(position));
	}

	return positions;
}

std::vector<Vector2f> GeometryLoader::GetUvs() const {
	auto uvsSource = meshData.GetPropertyWithBackup("polylist", "triangles")["input"].GetPropertyWithValue("-semantic", "TEXCOORD")["-source"].Get<std::string>().substr(1);
	auto uvsData = meshData["source"].GetPropertyWithValue("-id", uvsSource)["float_array"];
	auto uvsCount = uvsData["-count"].Get<uint32_t>();
	auto uvsRawData = String::Split(uvsData.Get<std::string>(), ' ');

	std::vector<Vector2f> uvs;
	uvs.reserve(uvsCount / 2);

	for (uint32_t i = 0; i < uvsCount / 2; i++) {
		Vector2f uv(String::From<float>(uvsRawData[2 * i]), 1.0f - String::From<float>(uvsRawData[2 * i + 1]));
		uvs.emplace_back(uv);
	}

	return uvs;
}

std::vector<Vector3f> GeometryLoader::GetNormals() const {
	auto normalsSource = meshData.GetPropertyWithBackup("polylist", "triangles")["input"].GetPropertyWithValue("-semantic", "NORMAL")["-source"].Get<std::string>().substr(1);
	auto normalsData = meshData["source"].GetPropertyWithValue("-id", normalsSource)["float_array"];
	auto normalsCount = normalsData["-count"].Get<uint32_t>();
	auto normalsRawData = String::Split(normalsData.Get<std::string>(), ' ');

	std::vector<Vector3f> normals;
	normals.reserve(normalsCount / 3);

	for (uint32_t i = 0; i < normalsCount / 3; i++) {
		Vector4f normal(String::From<float>(normalsRawData[3 * i]), String::From<float>(normalsRawData[3 * i + 1]), String::From<float>(normalsRawData[3 * i + 2]));
		normals.emplace_back(correction.Transform(normal));
	}

	return normals;
}
}
