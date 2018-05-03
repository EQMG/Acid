#include "Model.hpp"

#include "Helpers/FileSystem.hpp"

namespace fl
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
		std::vector<IVertex *> vertices = std::vector<IVertex *>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		LoadFromFile(filename, &vertices, &indices);

		if (!vertices.empty())
		{
			void *verticesData = vertices[0]->GetData(vertices);
			m_vertexBuffer = new VertexBuffer(vertices[0]->GetSize(), vertices.size(), verticesData);
			free(verticesData);
		}

		if (!indices.empty())
		{
			m_indexBuffer = new IndexBuffer(VK_INDEX_TYPE_UINT32, sizeof(indices[0]), indices.size(), indices.data());
		}

		m_aabb->Set(CalculateAabb(vertices));

		for (auto vertex : vertices)
		{
			delete vertex;
		}
	}

	Model::Model(std::vector<IVertex *> &vertices, std::vector<uint32_t> &indices, const std::string &name) :
		IResource(),
		m_filename(name),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_aabb(new ColliderAabb())
	{
		void *verticesData = vertices[0]->GetData(vertices);
		m_vertexBuffer = new VertexBuffer(vertices[0]->GetSize(), vertices.size(), verticesData);
		free(verticesData);

		m_indexBuffer = new IndexBuffer(VK_INDEX_TYPE_UINT32, sizeof(indices[0]), indices.size(), indices.data());

		m_aabb->Set(CalculateAabb(vertices));

		for (auto vertex : vertices)
		{
			delete vertex;
		}
	}

	Model::Model(std::vector<IVertex *> &vertices, const std::string &name) :
		IResource(),
		m_filename(name),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_aabb(new ColliderAabb())
	{
		void *verticesData = vertices[0]->GetData(vertices);
		m_vertexBuffer = new VertexBuffer(vertices[0]->GetSize(), vertices.size(), verticesData);
		free(verticesData);

		m_aabb->Set(CalculateAabb(vertices));

		for (auto vertex : vertices)
		{
			delete vertex;
		}
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

	void Model::Set(std::vector<IVertex *> &vertices, std::vector<uint32_t> &indices, const std::string &name)
	{
		m_filename = name;
		delete m_vertexBuffer;
		delete m_indexBuffer;

		if (!vertices.empty())
		{
			void *verticesData = vertices[0]->GetData(vertices);
			m_vertexBuffer = new VertexBuffer(vertices[0]->GetSize(), vertices.size(), verticesData);
			free(verticesData);
		}

		if (!indices.empty())
		{
			m_indexBuffer = new IndexBuffer(VK_INDEX_TYPE_UINT32, sizeof(indices[0]), indices.size(), indices.data());
		}

		m_aabb->Set(CalculateAabb(vertices));

		for (auto vertex : vertices)
		{
			delete vertex;
		}
	}

	void Model::LoadFromFile(const std::string &filename, std::vector<IVertex *> *vertices, std::vector<uint32_t> *indices)
	{
#if FL_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif

		delete m_indexBuffer;
		delete m_vertexBuffer;

		if (!FileSystem::FileExists(m_filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", m_filename.c_str());
			m_filename = FALLBACK_PATH;
			return;
		}

		const std::string fileLoaded = FileSystem::ReadTextFile(m_filename);
		std::vector<std::string> lines = FormatString::Split(fileLoaded, "\n");

		std::vector<uint32_t> indicesList = std::vector<uint32_t>();
		std::vector<VertexModelData *> verticesList = std::vector<VertexModelData *>();
		std::vector<Vector2> uvsList = std::vector<Vector2>();
		std::vector<Vector3> normalsList = std::vector<Vector3>();

		for (auto line : lines)
		{
			std::vector<std::string> split = FormatString::Split(line, " ", true);

			if (!split.empty())
			{
				const std::string prefix = split[0];

				if (prefix == "#")
				{
					continue;
				}
				else if (prefix == "v")
				{
					const Vector3 vertex = Vector3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3]));
					VertexModelData *newVertex = new VertexModelData(static_cast<int>(verticesList.size()), vertex);
					verticesList.push_back(newVertex);
				}
				else if (prefix == "vt")
				{
					const Vector2 uv = Vector2(std::stof(split[1]), 1.0f - std::stof(split[2]));
					uvsList.push_back(uv);
				}
				else if (prefix == "vn")
				{
					const Vector3 normal = Vector3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3]));
					normalsList.push_back(normal);
				}
				else if (prefix == "f")
				{
					// The split length of 3 faced + 1 for the f prefix.
					if (split.size() != 4 || FormatString::Contains(line, "//"))
					{
						fprintf(stderr, "Error reading the OBJ '%s', it does not appear to be UV mapped! The model will not be loaded.\n", m_filename.c_str());
						throw std::runtime_error("Model loading error.");
					}

					std::vector<std::string> vertex1 = FormatString::Split(split[1], "/");
					std::vector<std::string> vertex2 = FormatString::Split(split[2], "/");
					std::vector<std::string> vertex3 = FormatString::Split(split[3], "/");

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

		indices->swap(indicesList);

		// Turns the loaded data into a format that can be used by OpenGL.
		for (auto current : verticesList)
		{
			const Vector3 position = current->GetPosition();
			const Vector2 textures = uvsList[current->GetUvIndex()];
			const Vector3 normal = normalsList[current->GetNormalIndex()];
			const Vector3 tangent = current->GetAverageTangent();

			vertices->push_back(new VertexModel(position, textures, normal, tangent));

			delete current;
		}

#if FL_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();
		printf("Obj '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif

		m_filename = filename;
	}

	VertexModelData *Model::ProcessDataVertex(const Vector3 &vertex, std::vector<VertexModelData *> *vertices, std::vector<uint32_t> *indices)
	{
		const int index = static_cast<int>(vertex.m_x) - 1;
		const int textureIndex = static_cast<int>(vertex.m_y) - 1;
		const int normalIndex = static_cast<int>(vertex.m_z) - 1;
		VertexModelData *currentVertex = (*vertices)[index];

		if (!currentVertex->IsSet())
		{
			currentVertex->SetUvIndex(textureIndex);
			currentVertex->SetNormalIndex(normalIndex);
			indices->push_back(index);
			return currentVertex;
		}

		return DealWithAlreadyProcessedDataVertex(currentVertex, textureIndex, normalIndex, indices, vertices);
	}

	VertexModelData *Model::DealWithAlreadyProcessedDataVertex(VertexModelData *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<uint32_t> *indices, std::vector<VertexModelData *> *vertices)
	{
		if (previousVertex->HasSameTextureAndNormal(newTextureIndex, newNormalIndex))
		{
			indices->push_back(previousVertex->GetIndex());
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
		vertices->push_back(duplicateVertex);
		indices->push_back(duplicateVertex->GetIndex());
		return duplicateVertex;
	}

	void Model::CalculateTangents(VertexModelData *v0, VertexModelData *v1, VertexModelData *v2, std::vector<Vector2> *uvs)
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

		Vector3 *tangent = new Vector3(r * (deltaPos1 - deltaPos2));

		v0->AddTangent(tangent);
		v1->AddTangent(tangent);
		v2->AddTangent(tangent);
	}

	ColliderAabb Model::CalculateAabb(const std::vector<IVertex *> &vertices)
	{
		float minX = +std::numeric_limits<float>::infinity();
		float minY = +std::numeric_limits<float>::infinity();
		float minZ = +std::numeric_limits<float>::infinity();
		float maxX = -std::numeric_limits<float>::infinity();
		float maxY = -std::numeric_limits<float>::infinity();
		float maxZ = -std::numeric_limits<float>::infinity();

		for (auto vertex : vertices)
		{
			const Vector3 position = vertex->GetPosition();

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
