#include "GltfModel.hpp"

#include <tinygltf/tiny_gltf.h>

#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "Models/Vertex3d.hpp"

namespace acid {
const bool GltfModel::Registered = Register("gltf", ".gltf");

std::shared_ptr<GltfModel> GltfModel::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<GltfModel>(node))
		return resource;

	auto result = std::make_shared<GltfModel>("");
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<GltfModel> GltfModel::Create(const std::filesystem::path &filename) {
	GltfModel temp(filename, false);
	Node node;
	node << temp;
	return Create(node);
}

GltfModel::GltfModel(std::filesystem::path filename, bool load) :
	filename(std::move(filename)) {
	if (load) {
		Load();
	}
}

const Node &operator>>(const Node &node, GltfModel &model) {
	node["filename"].Get(model.filename);
	return node;
}

Node &operator<<(Node &node, const GltfModel &model) {
	node["filename"].Set(model.filename);
	return node;
}

void GltfModel::Load() {
	if (filename.empty()) {
		return;
	}

#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	auto folder = filename.parent_path();
	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("Model could not be loaded: ", filename, '\n');
		return;
	}

	tinygltf::Model gltfModel;
	tinygltf::TinyGLTF gltfContext;
	std::string warn, err;

	if (filename.extension() == ".glb") {
		if (!gltfContext.LoadBinaryFromMemory(&gltfModel, &err, &warn, reinterpret_cast<uint8_t *>(fileLoaded->data()), static_cast<uint32_t>(fileLoaded->size()))) {
			throw std::runtime_error(warn + err);
		}
	} else {
		if (!gltfContext.LoadASCIIFromString(&gltfModel, &err, &warn, fileLoaded->c_str(), static_cast<uint32_t>(fileLoaded->size()), folder.string())) {
			throw std::runtime_error(warn + err);
		}
	}

	std::vector<Vertex3d> vertices;
	std::vector<uint32_t> indices;
	std::unordered_map<Vertex3d, size_t> uniqueVertices;

	//LoadTextureSamplers(gltfModel);
	//LoadTextures(gltfModel);
	//LoadMaterials(gltfModel);
	auto scale = 1.0f;

	// TODO: Scene handling with no default scene.
	/*const tinygltf::Scene &scene = gltfModel.scenes[gltfModel.defaultScene > -1 ? gltfModel.defaultScene : 0];

	for (std::size_t i = 0; i < scene.nodes.size(); i++) {
		const tinygltf::Node node = gltfModel.nodes[scene.nodes[i]];
		LoadNode(nullptr, node, scene.nodes[i], gltfModel, indices, vertices, scale);
	}

	if (gltfModel.animations.size() > 0) {
		LoadAnimations(gltfModel);
	}

	LoadSkins(gltfModel);

	for (auto node : linearNodes) {
		// Assign skins.
		if (node->skinIndex > -1) {
			node->skin = skins[node->skinIndex];
		}

		// Initial pose.
		if (node->mesh) {
			node->update();
		}
	}

	auto extensions = gltfModel.extensionsUsed;*/

#ifdef ACID_DEBUG
	Log::Out("Model ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif

	Initialize(vertices, indices);
}
}
