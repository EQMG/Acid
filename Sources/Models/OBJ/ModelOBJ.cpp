#include "ModelOBJ.hpp"

#include <utility>
#include <unordered_map>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Files/FileSystem.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
	class MaterialStreamReader : public tinyobj::MaterialReader
	{
	public:
		explicit MaterialStreamReader(const std::string &folder) : 
			m_folder(folder)
		{
		}

		bool operator()(const std::string &matId, std::vector<tinyobj::material_t> *materials,
			std::map<std::string, int> *matMap, std::string *warn, std::string *err) override
		{
			(void)err;
			(void)matId;

			std::string filepath = m_folder + "/" + matId;

			if (!Files::ExistsInPath(filepath))
			{
				std::stringstream ss;
				ss << "Material stream in error state. \n";

				if (warn)
				{
					(*warn) += ss.str();
				}

				return false;
			}

			ifstream inStream(filepath);
			tinyobj::LoadMtl(matMap, materials, &inStream, warn, err);
			return true;
		}
	private:
		std::string m_folder;
	};

	std::shared_ptr<ModelOBJ> ModelOBJ::Create(const Metadata &metadata)
	{
		auto resource = Resources::Get()->Find(metadata);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelOBJ>(resource);
		}

		auto result = std::make_shared<ModelOBJ>("");
		Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<ModelOBJ> ModelOBJ::Create(const std::string &filename)
	{
		auto temp = ModelOBJ(filename, false);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	ModelOBJ::ModelOBJ(std::string filename, const bool &load) :
		m_filename(std::move(filename))
	{
		if (load)
		{
			ModelOBJ::Load();
		}
	}

	void ModelOBJ::Load()
	{
		if (m_filename.empty())
		{
			return;
		}

#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		auto folder = FileSystem::ParentDirectory(m_filename);
		ifstream inStream(m_filename);
		MaterialStreamReader materialReader(folder);

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &inStream, &materialReader))
		{
			throw std::runtime_error(warn + err);
		}

		std::vector<VertexModel> vertices = {};
		std::vector<uint32_t> indices = {};

		std::unordered_map<VertexModel, size_t> uniqueVertices = {};

		for (const auto &shape : shapes)
		{
			for (const auto &index : shape.mesh.indices)
			{
				Vector3 position = Vector3(
					attrib.vertices[3 * index.vertex_index],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				);

				Vector2 uv = Vector2(
					attrib.texcoords[2 * index.texcoord_index],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				);

				Vector3 normal = Vector3(
					attrib.normals[3 * index.normal_index],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				);

				VertexModel vertex = VertexModel(position, uv, normal);

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = vertices.size();
					vertices.emplace_back(vertex);
				}

				indices.emplace_back(static_cast<uint32_t>(uniqueVertices[vertex]));
			}
		}

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("Model OBJ '%s' loaded in %ims\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif

		Initialize(vertices, indices);
	}

	void ModelOBJ::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Filename", m_filename);
	}

	void ModelOBJ::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Type", "ModelOBJ");
		metadata.SetChild("Filename", m_filename);
	}
}
