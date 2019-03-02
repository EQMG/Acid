#include "ModelObj.hpp"

#include <utility>
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
		auto temp = ModelObj(filename, false);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	ModelObj::ModelObj(std::string filename, const bool &load) :
		m_filename(std::move(filename))
	{
		if (load)
		{
			ModelObj::Load();
		}
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

		ifstream inStream(m_filename);

		std::vector<uint32_t> indices = {};
		std::vector<std::unique_ptr<VertexModelData>> verticesList = {};
		std::vector<Vector3> positions = {};
		std::vector<Vector2> uvs = {};
		std::vector<Vector3> normals = {};

		size_t lineNum = 0;
		std::string linebuf;

		while (inStream.peek() != -1)
		{
			Files::SafeGetLine(inStream, linebuf);
			lineNum++;

			auto split = String::Split(linebuf, " \t");
			std::string firstToken = split[0];

			if (firstToken == "v")
			{
				positions.emplace_back(Vector3(String::From<float>(split[1]), String::From<float>(split[2]), String::From<float>(split[3])));
				
				verticesList.emplace_back(new VertexModelData(static_cast<uint32_t>(verticesList.size()), 
					Vector3(String::From<float>(split[1]), String::From<float>(split[2]), String::From<float>(split[3]))));
			}
			else if (firstToken == "vt")
			{
				uvs.emplace_back(Vector2(String::From<float>(split[1]), 1.0f - String::From<float>(split[2])));
			}
			else if (firstToken == "vn")
			{
				normals.emplace_back(Vector3(String::From<float>(split[1]), String::From<float>(split[2]), String::From<float>(split[3])));
			}
			else if (firstToken == "f")
			{
				auto vertex1 = String::Split(split[1], "/");
				auto vertex2 = String::Split(split[2], "/");
				auto vertex3 = String::Split(split[3], "/");

				VertexModelData *v0 = ProcessDataVertex(String::From<uint32_t>(vertex1[0]),
					String::From<std::optional<uint32_t>>(vertex1[1]), String::From<uint32_t>(vertex1[2]), verticesList, indices);
				VertexModelData *v1 = ProcessDataVertex(String::From<uint32_t>(vertex2[0]),
					String::From<std::optional<uint32_t>>(vertex2[1]), String::From<uint32_t>(vertex2[2]), verticesList, indices);
				VertexModelData *v2 = ProcessDataVertex(String::From<uint32_t>(vertex3[0]),
					String::From<std::optional<uint32_t>>(vertex3[1]), String::From<uint32_t>(vertex3[2]), verticesList, indices);
				CalculateTangents(v0, v1, v2, uvs);
			}
			else if (firstToken == "o")
			{
			}
			else if (firstToken == "mtllib")
			{
			}
			else if (firstToken == "usemtl")
			{
			}
		}

		std::vector<VertexModel> vertices = {};

		// Averages out vertex tangents, and disabled non set vertices,
		// and converts the loaded data into a format that can be used by models.
		for (auto &current : verticesList)
		{
			current->AverageTangents();
			auto vertexPosition = current->GetPosition();
		//	auto vertexPosition = current->GetPositionIndex() ? positions[*current->GetPositionIndex()] : Vector3::Zero;
			auto vertexUvs = current->GetUvIndex() ? uvs[*current->GetUvIndex()] : Vector2::Zero;
			auto vertexNormal = current->GetNormalIndex() ? normals[*current->GetNormalIndex()] : Vector3::Zero;
			auto vertexTangent = current->GetAverageTangent();
			vertices.emplace_back(VertexModel(vertexPosition, vertexUvs, vertexNormal, vertexTangent));
		}

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("Model OBJ '%s' loaded in %ims\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif

		Initialize(vertices, indices);
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

	VertexModelData *ModelObj::ProcessDataVertex(const uint32_t &vertexIndex, const std::optional<uint32_t> &uvIndex, const uint32_t &normalIndex,
		std::vector<std::unique_ptr<VertexModelData>> &vertices, std::vector<uint32_t> &indices)
	{
		if (!vertexIndex)
		{
			return nullptr;
		}

		auto currentVertex = vertices[vertexIndex - 1].get();

		if (!currentVertex->IsSet())
		{
			if (uvIndex)
			{
				currentVertex->SetUvIndex(*uvIndex - 1);
			}

			currentVertex->SetNormalIndex(normalIndex - 1);

			indices.emplace_back(vertexIndex - 1);
			return currentVertex;
		}

		return DealWithAlreadyProcessedDataVertex(currentVertex, uvIndex, normalIndex, vertices, indices);
	}

	VertexModelData *ModelObj::DealWithAlreadyProcessedDataVertex(VertexModelData *previousVertex, const std::optional<uint32_t> &newUvIndex, const uint32_t &newNormalIndex,
		std::vector<std::unique_ptr<VertexModelData>> &vertices, std::vector<uint32_t> &indices)
	{
		if (previousVertex->HasSameUvAndNormal(newUvIndex, newNormalIndex))
		{
			indices.emplace_back(previousVertex->GetIndex());
			return previousVertex;
		}

		auto *anotherVertex = previousVertex->GetDuplicateVertex();

		if (anotherVertex != nullptr)
		{
			return DealWithAlreadyProcessedDataVertex(anotherVertex, newUvIndex, newNormalIndex, vertices, indices);
		}

		auto *duplicateVertex = new VertexModelData(static_cast<uint32_t>(vertices.size()), previousVertex->GetPosition());
		
		if (newUvIndex)
		{
			duplicateVertex->SetUvIndex(*newUvIndex - 1);
		}

		duplicateVertex->SetNormalIndex(newNormalIndex - 1);

		previousVertex->SetDuplicateVertex(duplicateVertex);
		vertices.emplace_back(duplicateVertex);
		indices.emplace_back(duplicateVertex->GetIndex());
		return duplicateVertex;
	}

	void ModelObj::CalculateTangents(VertexModelData *v0, VertexModelData *v1, VertexModelData *v2, std::vector<Vector2> &uvs)
	{
		if (!v0->GetUvIndex() || !v1->GetUvIndex() || !v2->GetUvIndex())
		{
			return;
		}

		auto uv0 = uvs[*v0->GetUvIndex()];
		auto uv1 = uvs[*v1->GetUvIndex()];
		auto uv2 = uvs[*v2->GetUvIndex()];

		auto deltaUv1 = uv1 - uv0;
		auto deltaUv2 = uv2 - uv0;
		auto r = 1.0f / (deltaUv1.m_x * deltaUv2.m_y - deltaUv1.m_y * deltaUv2.m_x);

		auto deltaPos1 = v1->GetPosition() - v0->GetPosition();
		auto deltaPos2 = v2->GetPosition() - v0->GetPosition();
		deltaPos1 *= deltaUv2.m_y;
		deltaPos2 *= deltaUv1.m_y;

		auto tangent = Vector3(r * (deltaPos1 - deltaPos2));

		v0->AddTangent(tangent);
		v1->AddTangent(tangent);
		v2->AddTangent(tangent);
	}
}
