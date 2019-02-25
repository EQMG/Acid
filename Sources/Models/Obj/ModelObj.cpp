#include "ModelObj.hpp"

#include <cassert>
#include <utility>
#include "Files/FileSystem.hpp"
#include "Resources/Resources.hpp"

#define IS_SPACE(x) (((x) == ' ') || ((x) == '\t'))
#define IS_DIGIT(x) (static_cast<uint32_t>((x) - '0') < static_cast<uint32_t>(10))
#define IS_NEW_LINE(x) (((x) == '\r') || ((x) == '\n') || ((x) == '\0'))

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
		std::vector<Vector2> uvsList = {};
		std::vector<Vector3> normalsList = {};

		size_t lineNum = 0;
		std::string linebuf;

		while (inStream.peek() != -1)
		{
			Files::SafeGetLine(inStream, linebuf);
			lineNum++;

			const char *token = linebuf.c_str();
			token += strspn(token, " \t");

			if (token[0] == '\0' || token[0] == '#')
			{
				continue;
			}

			if (token[0] == 'v' && IS_SPACE(token[1]))
			{
				token += 2;
				verticesList.emplace_back(new VertexModelData(static_cast<uint32_t>(verticesList.size()), 
					Vector3(*ParseReal<float>(&token), *ParseReal<float>(&token), *ParseReal<float>(&token))));
			}
			else if (token[0] == 'v' && token[1] == 't' && IS_SPACE(token[2]))
			{
				token += 3;
				uvsList.emplace_back(Vector2(*ParseReal<float>(&token), 1.0f - *ParseReal<float>(&token)));
			}
			else if (token[0] == 'v' && token[1] == 'n' && IS_SPACE(token[2]))
			{
				token += 3;
				normalsList.emplace_back(Vector3(*ParseReal<float>(&token), *ParseReal<float>(&token), *ParseReal<float>(&token)));
			}
			else if (token[0] == 'f' && IS_SPACE(token[1]))
			{
				token += 2;
				token += strspn(token, " \t");

				/*auto startControl = " \t/";
				auto endControl = " \t\r/";

				auto v0 = ProcessDataVertex(ParseReal<uint32_t>(&token, startControl, endControl), ParseReal<uint32_t>(&token, startControl, endControl), 
					ParseReal<uint32_t>(&token, startControl, endControl), verticesList, indices);
				auto v1 = ProcessDataVertex(ParseReal<uint32_t>(&token, startControl, endControl), ParseReal<uint32_t>(&token, startControl, endControl),
					ParseReal<uint32_t>(&token, startControl, endControl), verticesList, indices);
				auto v2 = ProcessDataVertex(ParseReal<uint32_t>(&token, startControl, endControl), ParseReal<uint32_t>(&token, startControl, endControl),
					ParseReal<uint32_t>(&token, startControl, endControl), verticesList, indices);
				CalculateTangents(v0, v1, v2, uvsList);*/
				auto split = String::Split(linebuf, " ", true);

				// The split length of 3 faced + 1 for the f prefix.
			//	if (split.size() != 4 || String::Contains(linebuf, "//"))
			//	{
			//		Log::Error("Error reading the OBJ '%s', it does not appear to be UV mapped!\n", m_filename.c_str());
			//		assert(false && "Model loading error!");
			//	}

				auto vertex1 = String::Split(split[1], "/");
				auto vertex2 = String::Split(split[2], "/");
				auto vertex3 = String::Split(split[3], "/");

				VertexModelData *v0 = ProcessDataVertex(String::From<uint32_t>(vertex1[0]),
					String::From<uint32_t>(vertex1[1]), String::From<uint32_t>(vertex1[2]), verticesList, indices);
				VertexModelData *v1 = ProcessDataVertex(String::From<uint32_t>(vertex2[0]),
					String::From<uint32_t>(vertex2[1]), String::From<uint32_t>(vertex2[2]), verticesList, indices);
				VertexModelData *v2 = ProcessDataVertex(String::From<uint32_t>(vertex3[0]),
					String::From<uint32_t>(vertex3[1]), String::From<uint32_t>(vertex3[2]), verticesList, indices);
				CalculateTangents(v0, v1, v2, uvsList);
			}
			else if (token[0] == 'o' && IS_SPACE(token[1]))
			{
				token += 1;

			}
			else if (0 == strncmp(token, "mtllib", 6) && IS_SPACE(token[6]))
			{
				token += 7;
			}
			else if (0 == strncmp(token, "usemtl", 6) && IS_SPACE(token[6]))
			{
				token += 7;
			}
		}

		std::vector<VertexModel> vertices = {};

		// Averages out vertex tangents, and disabled non set vertices,
		// and converts the loaded data into a format that can be used by models.
		for (auto &current : verticesList)
		{
			current->AverageTangents();
			auto position = current->GetPosition();
			auto uvs = current->GetUvIndex() ? uvsList[*current->GetUvIndex()] : Vector2::Zero;
			auto normal = current->GetNormalIndex() ? normalsList[*current->GetNormalIndex()] : Vector3::Zero;
			auto tangent = current->GetAverageTangent();
			vertices.emplace_back(VertexModel(position, uvs, normal, tangent));
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

	VertexModelData *ModelObj::ProcessDataVertex(const std::optional<uint32_t> &vertexIndex, const std::optional<uint32_t> &uvIndex, const std::optional<uint32_t> &normalIndex,
		std::vector<std::unique_ptr<VertexModelData>> &vertices, std::vector<uint32_t> &indices)
	{
		if (!vertexIndex)
		{
			return nullptr;
		}

		auto currentVertex = vertices[*vertexIndex - 1].get();

		if (!currentVertex->IsSet())
		{
			if (uvIndex)
			{
				currentVertex->SetUvIndex(*uvIndex - 1);
			}

			if (normalIndex)
			{
				currentVertex->SetNormalIndex(*normalIndex - 1);
			}

			indices.emplace_back(*vertexIndex - 1);
			return currentVertex;
		}

		return DealWithAlreadyProcessedDataVertex(currentVertex, uvIndex, normalIndex, vertices, indices);
	}

	VertexModelData *ModelObj::DealWithAlreadyProcessedDataVertex(VertexModelData *previousVertex, const std::optional<uint32_t> &newUvIndex, const std::optional<uint32_t> &newNormalIndex,
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

		if (newNormalIndex)
		{
			duplicateVertex->SetNormalIndex(*newNormalIndex - 1);
		}

		previousVertex->SetDuplicateVertex(duplicateVertex);
		vertices.emplace_back(duplicateVertex);
		indices.emplace_back(duplicateVertex->GetIndex());
		return duplicateVertex;
	}

	void ModelObj::CalculateTangents(VertexModelData *v0, VertexModelData *v1, VertexModelData *v2, std::vector<Vector2> &uvs)
	{
		// TODO: Handle v0,v1,v2 being null, or a uvIndex not being set...
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
