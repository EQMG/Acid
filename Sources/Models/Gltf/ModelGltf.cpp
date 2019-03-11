#include "ModelGltf.hpp"

#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"
#include "Files/FileSystem.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelGltf> ModelGltf::Create(const Metadata &metadata)
	{
		auto resource = Resources::Get()->Find(metadata);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelGltf>(resource);
		}

		auto result = std::make_shared<ModelGltf>("");
		Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<ModelGltf> ModelGltf::Create(const std::string &filename)
	{
		auto temp = ModelGltf(filename, false);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	ModelGltf::ModelGltf(std::string filename, const bool &load) :
		m_filename(std::move(filename))
	{
		if (load)
		{
			ModelGltf::Load();
		}
	}

	void ModelGltf::Load()
	{
		if (m_filename.empty())
		{
			return;
		}

#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		auto folder = FileSystem::ParentDirectory(m_filename);
		auto fileLoaded = Files::Read(m_filename);

		if (!fileLoaded)
		{

			Log::Error("GLTF file could not be loaded: '%s'\n", m_filename.c_str());
			return;
		}

		tinygltf::Model gltfModel;
		tinygltf::TinyGLTF gltfContext;
		std::string warn, err;

		if (String::Lowercase(FileSystem::FileSuffix(m_filename)) == ".glb")
		{
			if (!gltfContext.LoadBinaryFromMemory(&gltfModel, &err, &warn, reinterpret_cast<uint8_t *>(fileLoaded->data()), static_cast<uint32_t>(fileLoaded->size())))
			{
				throw std::runtime_error(warn + err);
			}
		}
		else
		{
			if (!gltfContext.LoadASCIIFromString(&gltfModel, &err, &warn, fileLoaded->c_str(), fileLoaded->size(), folder))
			{
				throw std::runtime_error(warn + err);
			}
		}

		std::vector<VertexModel> vertices = {};
		std::vector<uint32_t> indices = {};

		std::unordered_map<VertexModel, size_t> uniqueVertices = {};

	//	LoadTextureSamplers(gltfModel);
	//	LoadTextures(gltfModel);
	//	LoadMaterials(gltfModel);
		float scale = 1.0f;

		// TODO: Scene handling with no default scene.
		/*const tinygltf::Scene &scene = gltfModel.scenes[gltfModel.defaultScene > -1 ? gltfModel.defaultScene : 0];

		for (size_t i = 0; i < scene.nodes.size(); i++)
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
		auto debugEnd = Engine::GetTime();
		Log::Out("Model GLTF '%s' loaded in %.3fms\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds<float>());
#endif

		Initialize(vertices, indices);
	}

	void ModelGltf::Decode(const Metadata & metadata)
	{
		metadata.GetChild("Filename", m_filename);
	}

	void ModelGltf::Encode(Metadata & metadata) const
	{
		metadata.SetChild<std::string>("Type", "ModelGltf");
		metadata.SetChild("Filename", m_filename);
	}
}
