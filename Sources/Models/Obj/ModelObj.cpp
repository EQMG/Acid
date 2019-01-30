#include "ModelObj.hpp"

#include <cassert>
#include "Files/FileSystem.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
	static const std::string FALLBACK_PATH = "Undefined.obj";

	std::shared_ptr<ModelObj> ModelObj::Create(const Metadata &metadata)
	{
		auto resource = Resources::Get()->Find(metadata);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelObj>(resource);
		}

		auto result = std::make_shared<ModelObj>("");
		Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<ModelObj> ModelObj::Create(const std::string &filename)
	{
		auto temp = ModelObj(filename);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	ModelObj::ModelObj(const std::string &filename) :
		m_filename(filename)
	{
		Load();
	}

	void ModelObj::Load()
	{
		if (m_filename.empty())
		{
			return;
		}

#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		auto fileLoaded = Files::Read(m_filename);

		if (!fileLoaded)
		{
			Log::Error("OBJ file could not be loaded: '%s'\n", m_filename.c_str());
			return;
		}

		auto lines = String::Split(*fileLoaded, "\n");

		std::vector<uint32_t> indices = {};
		std::vector<std::unique_ptr<VertexModelData>> verticesList = {};
		std::vector<Vector2> uvsList = {};
		std::vector<Vector3> normalsList = {};

		for (const auto &line : lines)
		{
			auto split = String::Split(line, " ", true);

			if (!split.empty())
			{
				std::string prefix = split[0];

				if (prefix == "#")
				{
					continue;
				}
				else if (prefix == "v")
				{
					Vector3 vertex = Vector3(String::From<float>(split[1]),
					                         String::From<float>(split[2]),
					                         String::From<float>(split[3]));
					VertexModelData *newVertex = new VertexModelData(
						static_cast<int>(verticesList.size()), vertex);
					verticesList.emplace_back(newVertex);
				}
				else if (prefix == "vt")
				{
					Vector2 uv = Vector2(String::From<float>(split[1]),
					                     1.0f - String::From<float>(split[2]));
					uvsList.emplace_back(uv);
				}
				else if (prefix == "vn")
				{
					Vector3 normal = Vector3(String::From<float>(split[1]),
					                         String::From<float>(split[2]),
					                         String::From<float>(split[3]));
					normalsList.emplace_back(normal);
				}
				else if (prefix == "f")
				{
					// The split length of 3 faced + 1 for the f prefix.
					if (split.size() != 4 || String::Contains(line, "//"))
					{
						Log::Error("Error reading the OBJ '%s', it does not appear to be UV mapped!\n", m_filename.c_str());
						assert(false && "Model loading error!");
					}

					auto vertex1 = String::Split(split[1], "/");
					auto vertex2 = String::Split(split[2], "/");
					auto vertex3 = String::Split(split[3], "/");

					VertexModelData *v0 = ProcessDataVertex(
						Vector3(String::From<float>(vertex1[0]), String::From<float>(vertex1[1]),
						        String::From<float>(vertex1[2])), verticesList, indices);
					VertexModelData *v1 = ProcessDataVertex(
						Vector3(String::From<float>(vertex2[0]), String::From<float>(vertex2[1]),
						        String::From<float>(vertex2[2])), verticesList, indices);
					VertexModelData *v2 = ProcessDataVertex(
						Vector3(String::From<float>(vertex3[0]), String::From<float>(vertex3[1]),
						        String::From<float>(vertex3[2])), verticesList, indices);
					CalculateTangents(v0, v1, v2, uvsList);
				}
				else if (prefix == "o")
				{
				}
				else if (prefix == "s")
				{
				}
				else
				{
					Log::Error("OBJ '%s' unknown line: '%s'\n", m_filename.c_str(), line.c_str());
				}
			}
		}

		std::vector<VertexModel> vertices = {};

		// Averages out vertex tangents, and disabled non set vertices,
		// and converts the loaded data into a format that can be used by models.
		for (auto &current : verticesList)
		{
			current->AverageTangents();

			if (!current->IsSet())
			{
				current->SetUvIndex(0);
				current->SetNormalIndex(0);
			}

			Vector3 position = current->GetPosition();
			Vector2 uvs = uvsList[current->GetUvIndex()];
			Vector3 normal = normalsList[current->GetNormalIndex()];
			Vector3 tangent = current->GetAverageTangent();
			vertices.emplace_back(VertexModel(position, uvs, normal, tangent));
		}

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("OBJ '%s' loaded in %ims\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif

		Model::Initialize(vertices, indices);
	}

	void ModelObj::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Filename", m_filename);
	}

	void ModelObj::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Type", "ModelObj");
		metadata.SetChild("Filename", m_filename);
	}

	VertexModelData *ModelObj::ProcessDataVertex(const Vector3 &vertex, std::vector<std::unique_ptr<VertexModelData>> &vertices, std::vector<uint32_t> &indices)
	{
		int32_t index = static_cast<int>(vertex.m_x) - 1;
		int32_t textureIndex = static_cast<int>(vertex.m_y) - 1;
		int32_t normalIndex = static_cast<int>(vertex.m_z) - 1;
		auto currentVertex = vertices[index].get();

		if (!currentVertex->IsSet())
		{
			currentVertex->SetUvIndex(textureIndex);
			currentVertex->SetNormalIndex(normalIndex);
			indices.emplace_back(index);
			return currentVertex;
		}

		return DealWithAlreadyProcessedDataVertex(currentVertex, textureIndex, normalIndex, vertices, indices);
	}

	VertexModelData *ModelObj::DealWithAlreadyProcessedDataVertex(VertexModelData *previousVertex, const int32_t &newTextureIndex, const int32_t &newNormalIndex, std::vector<std::unique_ptr<VertexModelData>> &vertices, std::vector<uint32_t> &indices)
	{
		if (previousVertex->HasSameTextureAndNormal(newTextureIndex, newNormalIndex))
		{
			indices.emplace_back(previousVertex->GetIndex());
			return previousVertex;
		}

		VertexModelData *anotherVertex = previousVertex->GetDuplicateVertex();

		if (anotherVertex != nullptr)
		{
			return DealWithAlreadyProcessedDataVertex(anotherVertex, newTextureIndex, newNormalIndex, vertices, indices);
		}

		VertexModelData *duplicateVertex = new VertexModelData(static_cast<uint32_t>(vertices.size()), previousVertex->GetPosition());
		duplicateVertex->SetUvIndex(newTextureIndex);
		duplicateVertex->SetNormalIndex(newNormalIndex);
		previousVertex->SetDuplicateVertex(duplicateVertex);
		vertices.emplace_back(duplicateVertex);
		indices.emplace_back(duplicateVertex->GetIndex());
		return duplicateVertex;
	}

	void ModelObj::CalculateTangents(VertexModelData *v0, VertexModelData *v1, VertexModelData *v2, std::vector<Vector2> &uvs)
	{
		Vector2 uv0 = uvs[v0->GetUvIndex()];
		Vector2 uv1 = uvs[v1->GetUvIndex()];
		Vector2 uv2 = uvs[v2->GetUvIndex()];

		Vector2 deltaUv1 = uv1 - uv0;
		Vector2 deltaUv2 = uv2 - uv0;
		float r = 1.0f / (deltaUv1.m_x * deltaUv2.m_y - deltaUv1.m_y * deltaUv2.m_x);

		Vector3 deltaPos1 = v1->GetPosition() - v0->GetPosition();
		Vector3 deltaPos2 = v2->GetPosition() - v0->GetPosition();
		deltaPos1 *= deltaUv2.m_y;
		deltaPos2 *= deltaUv1.m_y;

		Vector3 tangent = Vector3(r * (deltaPos1 - deltaPos2));

		v0->AddTangent(tangent);
		v1->AddTangent(tangent);
		v2->AddTangent(tangent);
	}
}
