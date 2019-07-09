#include "ModelObj.hpp"

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"
#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexDefault.hpp"

namespace acid
{
class MaterialStreamReader :
	public tinyobj::MaterialReader
{
public:
	explicit MaterialStreamReader(std::filesystem::path folder) :
		m_folder{std::move(folder)}
	{
	}

	bool operator()(const std::string &matId, std::vector<tinyobj::material_t> *materials, std::map<std::string, int> *matMap, std::string *warn, std::string *err) override
	{
		(void) err;
		(void) matId;

		auto filepath{m_folder / matId};

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

		IFStream inStream{filepath};
		tinyobj::LoadMtl(matMap, materials, &inStream, warn, err);
		return true;
	}

private:
	std::filesystem::path m_folder;
};

std::shared_ptr<ModelObj> ModelObj::Create(const Metadata &metadata)
{
	auto resource{Resources::Get()->Find(metadata)};

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<ModelObj>(resource);
	}

	auto result{std::make_shared<ModelObj>("")};
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelObj> ModelObj::Create(const std::filesystem::path &filename)
{
	ModelObj temp{filename, false};
	Metadata metadata;
	metadata << temp;
	return Create(metadata);
}

ModelObj::ModelObj(std::filesystem::path filename, const bool &load) :
	m_filename(std::move(filename))
{
	if (load)
	{
		Load();
	}
}

const Metadata &operator>>(const Metadata &metadata, ModelObj &model)
{
	metadata.GetChild("filename", model.m_filename);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ModelObj &model)
{
	metadata.SetChild<std::string>("type", "ModelObj");
	metadata.SetChild("filename", model.m_filename);
	return metadata;
}

void ModelObj::Load()
{
	if (m_filename.empty())
	{
		return;
	}

#if defined(ACID_VERBOSE)
	auto debugStart{Time::Now()};
#endif

	auto folder{m_filename.parent_path()};
	IFStream inStream(m_filename);
	MaterialStreamReader materialReader(folder);

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &inStream, &materialReader))
	{
		throw std::runtime_error{warn + err};
	}

	std::vector<VertexDefault> vertices;
	std::vector<uint32_t> indices;
	std::unordered_map<VertexDefault, size_t> uniqueVertices;

	for (const auto &shape : shapes)
	{
		for (const auto &index : shape.mesh.indices)
		{
			Vector3f position{attrib.vertices[3 * index.vertex_index], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2]};
			Vector2f uv{attrib.texcoords[2 * index.texcoord_index], 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};
			Vector3f normal{attrib.normals[3 * index.normal_index], attrib.normals[3 * index.normal_index + 1], attrib.normals[3 * index.normal_index + 2]};
			VertexDefault vertex{position, uv, normal};

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = vertices.size();
				vertices.emplace_back(vertex);
			}

			indices.emplace_back(static_cast<uint32_t>(uniqueVertices[vertex]));
		}
	}

#if defined(ACID_VERBOSE)
	std::cout << "Model OBJ " << m_filename << " loaded in " << (Time::Now() - debugStart).AsMilliseconds<float>() << '\n';
#endif

	Initialize(vertices, indices);
}
}
