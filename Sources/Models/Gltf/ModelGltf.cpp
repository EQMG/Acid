#include "ModelGltf.hpp"

#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_IMPLEMENTATION

#include "tiny_gltf.h"
#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexDefault.hpp"

namespace acid {
std::shared_ptr<ModelGltf> ModelGltf::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find(node)) {
		return std::dynamic_pointer_cast<ModelGltf>(resource);
	}

	auto result = std::make_shared<ModelGltf>("");
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelGltf> ModelGltf::Create(const std::filesystem::path &filename) {
	ModelGltf temp(filename, false);
	Node node;
	node << temp;
	return Create(node);
}

ModelGltf::ModelGltf(std::filesystem::path filename, bool load) :
	m_filename(std::move(filename)) {
	if (load) {
		Load();
	}
}

const Node &operator>>(const Node &node, ModelGltf &model) {
	node["filename"].Get(model.m_filename);
	return node;
}

Node &operator<<(Node &node, const ModelGltf &model) {
	node["type"].Set("ModelGltf");
	node["filename"].Set(model.m_filename);
	return node;
}

void ModelGltf::Load() {
	if (m_filename.empty()) {
		return;
	}

#if defined(ACID_VERBOSE)
	auto debugStart = Time::Now();
#endif

	auto folder = m_filename.parent_path();
	auto fileLoaded = Files::Read(m_filename);

	if (!fileLoaded) {
		Log::Error("GLTF file could not be loaded: ", m_filename, '\n');
		return;
	}

	tinygltf::Model gltfModel;
	tinygltf::TinyGLTF gltfContext;
	std::string warn, err;

	if (m_filename.extension() == ".glb") {
		if (!gltfContext.LoadBinaryFromMemory(&gltfModel, &err, &warn, reinterpret_cast<uint8_t *>(fileLoaded->data()), static_cast<uint32_t>(fileLoaded->size()))) {
			throw std::runtime_error(warn + err);
		}
	} else {
		if (!gltfContext.LoadASCIIFromString(&gltfModel, &err, &warn, fileLoaded->c_str(), static_cast<uint32_t>(fileLoaded->size()), folder.string())) {
			throw std::runtime_error(warn + err);
		}
	}

	std::vector<VertexDefault> vertices;
	std::vector<uint32_t> indices;
	std::unordered_map<VertexDefault, size_t> uniqueVertices;

	//LoadTextureSamplers(gltfModel);
	//LoadTextures(gltfModel);
	//LoadMaterials(gltfModel);
	auto scale = 1.0f;

	// TODO: Scene handling with no default scene.
	/*const tinygltf::Scene &scene = gltfModel.scenes[gltfModel.defaultScene > -1 ? gltfModel.defaultScene : 0];

	for (std::size_t i = 0; i < scene.nodes.size(); i++)
	{
		const tinygltf::Node node = gltfModel.nodes[scene.nodes[i]];
		LoadNode(nullptr, node, scene.nodes[i], gltfModel, indices, vertices, scale);
	}

	if (gltfModel.animations.size() > 0)
	{
		LoadAnimations(gltfModel);
	}

	LoadSkins(gltfModel);

	for (auto node : linearNodes) {
		// Assign skins.
		if (node->skinIndex > -1)
		{
			node->skin = skins[node->skinIndex];
		}

		// Initial pose.
		if (node->mesh)
		{
			node->update();
		}
	}

	auto extensions = gltfModel.extensionsUsed;*/

#if defined(ACID_VERBOSE)
	Log::Out("Model GLTF ", m_filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif

	Initialize(vertices, indices);
}
}
