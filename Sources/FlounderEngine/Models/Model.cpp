#include "Model.hpp"

#include <cassert>
#include "Helpers/FileSystem.hpp"

namespace Flounder
{
	static const std::string FALLBACK_PATH = "Resources/Undefined.obj";

	Model::Model() :
		IResource(),
		m_filename(""),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_aabb(new ColliderAabb())
	{
	}

	Model::Model(const std::string &filename) :
		IResource(),
		m_filename(filename),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_aabb(new ColliderAabb())
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		ModelLoaded modelLoaded = LoadFromFile(filename);

		if (!modelLoaded.vertices.empty())
		{
			m_vertexBuffer = new VertexBuffer(sizeof(modelLoaded.vertices[0]), modelLoaded.vertices.size(), modelLoaded.vertices.data());
		}

		if (!modelLoaded.indices.empty())
		{
			m_indexBuffer = new IndexBuffer(VK_INDEX_TYPE_UINT32, sizeof(modelLoaded.indices[0]), modelLoaded.indices.size(), modelLoaded.indices.data());
		}

		m_aabb->Set(CalculateAabb(vertices));
	}

	Model::Model(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices, const std::string &name) :
		IResource(),
		m_filename(name),
		m_vertexBuffer(new VertexBuffer(sizeof(vertices[0]), vertices.size(), vertices.data())),
		m_indexBuffer(new IndexBuffer(VK_INDEX_TYPE_UINT32, sizeof(indices[0]), indices.size(), indices.data())),
		m_aabb(new ColliderAabb())
	{
		m_aabb->Set(CalculateAabb(vertices));
	}

	Model::Model(std::vector<Vertex> &vertices, const std::string &name) :
		IResource(),
		m_filename(name),
		m_vertexBuffer(new VertexBuffer(sizeof(vertices[0]), vertices.size(), vertices.data())),
		m_indexBuffer(nullptr),
		m_aabb(new ColliderAabb())
	{
		m_aabb->Set(CalculateAabb(vertices));
	}

	Model::~Model()
	{
		delete m_indexBuffer;
		delete m_vertexBuffer;
		delete m_aabb;
	}

	void Model::CmdRender(const VkCommandBuffer &commandBuffer, const unsigned int &instances)
	{
		if (m_vertexBuffer != nullptr && m_indexBuffer != nullptr)
		{
			VkBuffer vertexBuffers[] = {m_vertexBuffer->GetBuffer()};
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->GetBuffer(), 0, m_indexBuffer->GetIndexType());
			vkCmdDrawIndexed(commandBuffer, m_indexBuffer->GetIndexCount(), instances, 0, 0, 0);
		}
		else if (m_vertexBuffer != nullptr && m_indexBuffer == nullptr)
		{
			VkBuffer vertexBuffers[] = {m_vertexBuffer->GetBuffer()};
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			vkCmdDraw(commandBuffer, m_vertexBuffer->GetVertexCount(), instances, 0, 0);
		}
		//	else
		//	{
		//		assert(false && "Cannot render model, no buffers exist for it!");
		//	}
	}

	void Model::Set(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices, const std::string &name)
	{
		m_filename = name;
		delete m_vertexBuffer;
		delete m_indexBuffer;
		m_vertexBuffer = new VertexBuffer(sizeof(vertices[0]), vertices.size(), vertices.data());
		m_indexBuffer = new IndexBuffer(VK_INDEX_TYPE_UINT32, sizeof(indices[0]), indices.size(), indices.data());
		m_aabb->Set(CalculateAabb(vertices));
	}

	ModelLoaded Model::LoadFromFile(const std::string &filename)
	{
#if FLOUNDER_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif

		delete m_indexBuffer;
		delete m_vertexBuffer;

		ModelLoaded modelLoaded = ModelLoaded{
			std::vector<Vertex>(),
			std::vector<uint32_t>()
		};

		if (!FileSystem::FileExists(m_filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", m_filename.c_str());
			m_filename = FALLBACK_PATH;
			return modelLoaded;
		}

		const std::string fileLoaded = FileSystem::ReadTextFile(m_filename);
		std::vector<std::string> lines = FormatString::Split(fileLoaded, "\n");

		std::vector<uint32_t> indicesList = std::vector<uint32_t>();
		std::vector<VertexData *> verticesList = std::vector<VertexData *>();
		std::vector<Vector2> uvsList = std::vector<Vector2>();
		std::vector<Vector3> normalsList = std::vector<Vector3>();

		for (auto it = lines.begin(); it < lines.end(); ++it)
		{
			std::string line = FormatString::Trim(*it);

			std::vector<std::string> split = FormatString::Split(line, " ");

			if (!split.empty())
			{
				const std::string prefix = split.at(0);

				if (prefix == "#")
				{
					continue;
				}
				else if (prefix == "v")
				{
					const Vector3 vertex = Vector3(stof(split.at(1)), stof(split.at(2)), stof(split.at(3)));
					VertexData *newVertex = new VertexData(static_cast<int>(verticesList.size()), vertex);
					verticesList.push_back(newVertex);
				}
				else if (prefix == "vt")
				{
					const Vector2 texture = Vector2(stof(split.at(1)), 1.0f - stof(split.at(2)));
					uvsList.push_back(texture);
				}
				else if (prefix == "vn")
				{
					const Vector3 normal = Vector3(stof(split.at(1)), stof(split.at(2)), stof(split.at(3)));
					normalsList.push_back(normal);
				}
				else if (prefix == "f")
				{
					// The split length of 3 faced + 1 for the f prefix.
					if (split.size() != 4 || FormatString::Contains(line, "//"))
					{
						fprintf(stderr, "Error reading the OBJ '%s', it does not appear to be UV mapped! The model will not be loaded.\n", m_filename.c_str());
						assert(false);
					}

					std::vector<std::string> vertex1 = FormatString::Split(split.at(1), "/");
					std::vector<std::string> vertex2 = FormatString::Split(split.at(2), "/");
					std::vector<std::string> vertex3 = FormatString::Split(split.at(3), "/");

					VertexData *v0 = ProcessDataVertex(Vector3(stof(vertex1.at(0)), stof(vertex1.at(1)), stof(vertex1.at(2))), &verticesList, &indicesList);
					VertexData *v1 = ProcessDataVertex(Vector3(stof(vertex2.at(0)), stof(vertex2.at(1)), stof(vertex2.at(2))), &verticesList, &indicesList);
					VertexData *v2 = ProcessDataVertex(Vector3(stof(vertex3.at(0)), stof(vertex3.at(1)), stof(vertex3.at(2))), &verticesList, &indicesList);
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
					fprintf(stderr, "OBJ '%s' unknown line: '%s'\n", m_filename.c_str(), line.c_str());
				}
			}
		}

		// Averages out vertex tangents, and disabled non set vertices,
		for (auto current : verticesList)
		{
			current->AverageTangents();

			if (!current->IsSet())
			{
				current->SetUvIndex(0);
				current->SetNormalIndex(0);
			}
		}

		modelLoaded.indices.swap(indicesList);

		// Turns the loaded OBJ data into a formal that can be used by OpenGL.
		for (auto current : verticesList)
		{
			const Vector3 position = current->GetPosition();
			const Vector2 textures = uvsList.at(current->GetUvIndex());
			const Vector3 normal = normalsList.at(current->GetNormalIndex());
			const Vector3 tangent = current->GetAverageTangent();

			const Vertex vertex = Vertex(position, textures, normal, tangent);

			modelLoaded.vertices.push_back(vertex);

			delete current;
		}

#if FLOUNDER_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();
		printf("Obj '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif

		m_filename = filename;
		return modelLoaded;
	}

	VertexData *Model::ProcessDataVertex(const Vector3 &vertex, std::vector<VertexData *> *vertices, std::vector<uint32_t> *indices)
	{
		const int index = static_cast<int>(vertex.m_x) - 1;
		const int textureIndex = static_cast<int>(vertex.m_y) - 1;
		const int normalIndex = static_cast<int>(vertex.m_z) - 1;
		VertexData *currentVertex = vertices->at(index);

		if (!currentVertex->IsSet())
		{
			currentVertex->SetUvIndex(textureIndex);
			currentVertex->SetNormalIndex(normalIndex);
			indices->push_back(index);
			return currentVertex;
		}

		return DealWithAlreadyProcessedDataVertex(currentVertex, textureIndex, normalIndex, indices, vertices);
	}

	VertexData *Model::DealWithAlreadyProcessedDataVertex(VertexData *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<uint32_t> *indices, std::vector<VertexData *> *vertices)
	{
		if (previousVertex->HasSameTextureAndNormal(newTextureIndex, newNormalIndex))
		{
			indices->push_back(previousVertex->GetIndex());
			return previousVertex;
		}

		VertexData *anotherVertex = previousVertex->GetDuplicateVertex();

		if (anotherVertex != nullptr)
		{
			return DealWithAlreadyProcessedDataVertex(anotherVertex, newTextureIndex, newNormalIndex, indices, vertices);
		}

		VertexData *duplicateVertex = new VertexData(static_cast<uint32_t>(vertices->size()), previousVertex->GetPosition());
		duplicateVertex->SetUvIndex(newTextureIndex);
		duplicateVertex->SetNormalIndex(newNormalIndex);
		previousVertex->SetDuplicateVertex(duplicateVertex);
		vertices->push_back(duplicateVertex);
		indices->push_back(duplicateVertex->GetIndex());
		return duplicateVertex;
	}

	void Model::CalculateTangents(VertexData *v0, VertexData *v1, VertexData *v2, std::vector<Vector2> *uvs)
	{
		const Vector2 uv0 = uvs->at(v0->GetUvIndex());
		const Vector2 uv1 = uvs->at(v1->GetUvIndex());
		const Vector2 uv2 = uvs->at(v2->GetUvIndex());

		Vector2 *deltaUv1 = Vector2::Subtract(uv1, uv0, nullptr);
		Vector2 *deltaUv2 = Vector2::Subtract(uv2, uv0, nullptr);
		const float r = 1.0f / (deltaUv1->m_x * deltaUv2->m_y - deltaUv1->m_y * deltaUv2->m_x);

		Vector3 *deltaPos1 = Vector3::Subtract(v1->GetPosition(), v0->GetPosition(), nullptr);
		Vector3 *deltaPos2 = Vector3::Subtract(v2->GetPosition(), v0->GetPosition(), nullptr);
		deltaPos1->Scale(deltaUv2->m_y);
		deltaPos2->Scale(deltaUv1->m_y);

		Vector3 *tangent = Vector3::Subtract(*deltaPos1, *deltaPos2, nullptr);
		tangent->Scale(r);

		v0->AddTangent(tangent);
		v1->AddTangent(tangent);
		v2->AddTangent(tangent);

		delete deltaPos1;
		delete deltaPos2;
		delete deltaUv1;
		delete deltaUv2;
	}

	ColliderAabb Model::CalculateAabb(const std::vector<Vertex> &vertices)
	{
		float minX = +std::numeric_limits<float>::infinity();
		float minY = +std::numeric_limits<float>::infinity();
		float minZ = +std::numeric_limits<float>::infinity();
		float maxX = -std::numeric_limits<float>::infinity();
		float maxY = -std::numeric_limits<float>::infinity();
		float maxZ = -std::numeric_limits<float>::infinity();

		for (const auto &vertex : vertices)
		{
			const Vector3 position = vertex.m_position;

			if (position.m_x < minX)
			{
				minX = position.m_x;
			}
			else if (position.m_x > maxX)
			{
				maxX = position.m_x;
			}

			if (position.m_y < minY)
			{
				minY = position.m_y;
			}
			else if (position.m_y > maxY)
			{
				maxY = position.m_y;
			}

			if (position.m_z < minZ)
			{
				minZ = position.m_z;
			}
			else if (position.m_z > maxZ)
			{
				maxZ = position.m_z;
			}
		}

		return ColliderAabb(Vector3(minX, minY, minZ), Vector3(maxX, maxY, maxZ));
	}
}
