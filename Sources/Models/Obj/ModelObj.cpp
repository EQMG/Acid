#include "ModelObj.hpp"

#include "Helpers/FileSystem.hpp"

namespace acid
{
	ModelObj::ModelObj(const std::string &filename) :
		Model()
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		auto fileLoaded = FileSystem::ReadTextFile(filename);

		if (!fileLoaded)
		{
			return;
		}

		auto lines = FormatString::Split(*fileLoaded, "\n");

		auto indicesList = std::vector<uint32_t>();
		auto verticesList = std::vector<VertexModelData *>();
		auto uvsList = std::vector<Vector2>();
		auto normalsList = std::vector<Vector3>();

		for (auto &line : lines)
		{
			auto split = FormatString::Split(line, " ", true);

			if (!split.empty())
			{
				std::string prefix = split[0];

				if (prefix == "#")
				{
					continue;
				}
				else if (prefix == "v")
				{
					Vector3 vertex = Vector3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3]));
					VertexModelData *newVertex = new VertexModelData(static_cast<int>(verticesList.size()), vertex);
					verticesList.emplace_back(newVertex);
				}
				else if (prefix == "vt")
				{
					Vector2 uv = Vector2(std::stof(split[1]), 1.0f - std::stof(split[2]));
					uvsList.emplace_back(uv);
				}
				else if (prefix == "vn")
				{
					Vector3 normal = Vector3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3]));
					normalsList.emplace_back(normal);
				}
				else if (prefix == "f")
				{
					// The split length of 3 faced + 1 for the f prefix.
					if (split.size() != 4 || FormatString::Contains(line, "//"))
					{
						fprintf(stderr, "Error reading the OBJ '%s', it does not appear to be UV mapped! The model will not be loaded.\n", filename.c_str());
						throw std::runtime_error("Model loading error.");
					}

					auto vertex1 = FormatString::Split(split[1], "/");
					auto vertex2 = FormatString::Split(split[2], "/");
					auto vertex3 = FormatString::Split(split[3], "/");

					VertexModelData *v0 = ProcessDataVertex(Vector3(std::stof(vertex1[0]), std::stof(vertex1[1]), std::stof(vertex1[2])), &verticesList, &indicesList);
					VertexModelData *v1 = ProcessDataVertex(Vector3(std::stof(vertex2[0]), std::stof(vertex2[1]), std::stof(vertex2[2])), &verticesList, &indicesList);
					VertexModelData *v2 = ProcessDataVertex(Vector3(std::stof(vertex3[0]), std::stof(vertex3[1]), std::stof(vertex3[2])), &verticesList, &indicesList);
					CalculateTangents(v0, v1, v2, &uvsList);
				}
				else if (prefix == "o")
				{
				}
				else if (prefix == "s")
				{
				}
				else
				{
					fprintf(stderr, "OBJ '%s' unknown line: '%s'\n", filename.c_str(), line.c_str());
				}
			}
		}

		// Averages out vertex tangents, and disabled non set vertices,
		for (auto &current : verticesList)
		{
			current->AverageTangents();

			if (!current->IsSet())
			{
				current->SetUvIndex(0);
				current->SetNormalIndex(0);
			}
		}

		std::vector<IVertex *> vertices = std::vector<IVertex *>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		indices.swap(indicesList);

		// Turns the loaded data into a format that can be used by OpenGL.
		for (auto &current : verticesList)
		{
			Vector3 position = current->GetPosition();
			Vector2 textures = uvsList[current->GetUvIndex()];
			Vector3 normal = normalsList[current->GetNormalIndex()];
			Vector3 tangent = current->GetAverageTangent();

			vertices.emplace_back(new VertexModel(position, textures, normal, tangent));

			delete current;
		}

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Obj '%s' loaded in %fms\n", filename.c_str(), debugEnd - debugStart);
#endif

		Model::Set(vertices, indices, filename);
	}


	VertexModelData *ModelObj::ProcessDataVertex(const Vector3 &vertex, std::vector<VertexModelData *> *vertices, std::vector<uint32_t> *indices)
	{
		int index = static_cast<int>(vertex.m_x) - 1;
		int textureIndex = static_cast<int>(vertex.m_y) - 1;
		int normalIndex = static_cast<int>(vertex.m_z) - 1;
		VertexModelData *currentVertex = (*vertices)[index];

		if (!currentVertex->IsSet())
		{
			currentVertex->SetUvIndex(textureIndex);
			currentVertex->SetNormalIndex(normalIndex);
			indices->emplace_back(index);
			return currentVertex;
		}

		return DealWithAlreadyProcessedDataVertex(currentVertex, textureIndex, normalIndex, indices, vertices);
	}

	VertexModelData *ModelObj::DealWithAlreadyProcessedDataVertex(VertexModelData *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<uint32_t> *indices, std::vector<VertexModelData *> *vertices)
	{
		if (previousVertex->HasSameTextureAndNormal(newTextureIndex, newNormalIndex))
		{
			indices->emplace_back(previousVertex->GetIndex());
			return previousVertex;
		}

		VertexModelData *anotherVertex = previousVertex->GetDuplicateVertex();

		if (anotherVertex != nullptr)
		{
			return DealWithAlreadyProcessedDataVertex(anotherVertex, newTextureIndex, newNormalIndex, indices, vertices);
		}

		VertexModelData *duplicateVertex = new VertexModelData(static_cast<uint32_t>(vertices->size()), previousVertex->GetPosition());
		duplicateVertex->SetUvIndex(newTextureIndex);
		duplicateVertex->SetNormalIndex(newNormalIndex);
		previousVertex->SetDuplicateVertex(duplicateVertex);
		vertices->emplace_back(duplicateVertex);
		indices->emplace_back(duplicateVertex->GetIndex());
		return duplicateVertex;
	}

	void ModelObj::CalculateTangents(VertexModelData *v0, VertexModelData *v1, VertexModelData *v2, std::vector<Vector2> *uvs)
	{
		Vector2 uv0 = (*uvs)[v0->GetUvIndex()];
		Vector2 uv1 = (*uvs)[v1->GetUvIndex()];
		Vector2 uv2 = (*uvs)[v2->GetUvIndex()];

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
