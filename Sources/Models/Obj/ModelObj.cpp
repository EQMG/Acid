#include "ModelObj.hpp"

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"
#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexDefault.hpp"

namespace acid {
class MaterialStreamReader :
	public tinyobj::MaterialReader {
public:
	explicit MaterialStreamReader(std::filesystem::path folder) :
		m_folder(std::move(folder)) { }

	bool operator()(const std::string &matId, std::vector<tinyobj::material_t> *materials, std::map<std::string, int> *matMap, std::string *warn, std::string *err) override {
		(void)err;
		(void)matId;

		auto filepath = m_folder / matId;

		if (!Files::ExistsInPath(filepath)) {
			std::stringstream ss;
			ss << "Material stream in error state. \n";

			if (warn) {
				(*warn) += ss.str();
			}

			return false;
		}

		IFStream inStream(filepath);
		tinyobj::LoadMtl(matMap, materials, &inStream, warn, err);
		return true;
	}

private:
	std::filesystem::path m_folder;
};

std::shared_ptr<ModelObj> ModelObj::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find(node)) {
		return std::dynamic_pointer_cast<ModelObj>(resource);
	}

	auto result = std::make_shared<ModelObj>("");
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelObj> ModelObj::Create(const std::filesystem::path &filename) {
	ModelObj temp(filename, false);
	Node node;
	node << temp;
	return Create(node);
}

ModelObj::ModelObj(std::filesystem::path filename, bool load) :
	m_filename(std::move(filename)) {
	if (load) {
		Load();
	}
}

const Node &operator>>(const Node &node, ModelObj &model) {
	node["filename"].Get(model.m_filename);
	return node;
}

Node &operator<<(Node &node, const ModelObj &model) {
	node["type"].Set("ModelObj");
	node["filename"].Set(model.m_filename);
	return node;
}

void ModelObj::Load() {
	if (m_filename.empty()) {
		return;
	}

#if defined(ACID_VERBOSE)
	auto debugStart = Time::Now();
#endif

	auto folder = m_filename.parent_path();
	IFStream inStream(m_filename);
	MaterialStreamReader materialReader(folder);

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &inStream, &materialReader)) {
		throw std::runtime_error(warn + err);
	}

	std::vector<VertexDefault> vertices;
	std::vector<uint32_t> indices;
	std::unordered_map<VertexDefault, size_t> uniqueVertices;

	for (const auto &shape : shapes) {
		for (const auto &index : shape.mesh.indices) {
			Vector3f position(attrib.vertices[3 * index.vertex_index], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2]);
			Vector2f uv(attrib.texcoords[2 * index.texcoord_index], 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);
			Vector3f normal(attrib.normals[3 * index.normal_index], attrib.normals[3 * index.normal_index + 1], attrib.normals[3 * index.normal_index + 2]);
			VertexDefault vertex(position, uv, normal);

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = vertices.size();
				vertices.emplace_back(vertex);
			}

			indices.emplace_back(static_cast<uint32_t>(uniqueVertices[vertex]));
		}
	}

#if defined(ACID_VERBOSE)
	Log::Out("Model OBJ ", m_filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif

	Initialize(vertices, indices);
}
}
