#include "Model.hpp"

namespace Flounder
{
	Model::Model(const std::string &file) :
		m_file(file),
		m_indices(nullptr),
		m_vertices(nullptr),
		m_textures(nullptr),
		m_normals(nullptr),
		m_tangents(nullptr),
		m_aabb(nullptr),
		m_verticesBuffer(VK_NULL_HANDLE),
		m_texturesBuffer(VK_NULL_HANDLE),
		m_normalsBuffer(VK_NULL_HANDLE),
		m_tangentsBuffer(VK_NULL_HANDLE),
		m_memory(VK_NULL_HANDLE),
		m_bufferInfo({})
	{
		LoadFromFile();
	}

	Model::Model(const std::vector<int> &indices, const std::vector<float> &vertices, const std::vector<float> &textures, const std::vector<float> &normals, const std::vector<float> &tangents)
	{
		m_file = "";

		m_indices = new std::vector<int>(indices);
		m_vertices = new std::vector<float>(vertices);
		m_textures = new std::vector<float>(textures);
		m_normals = new std::vector<float>(normals);
		m_tangents = new std::vector<float>(tangents);

		m_aabb = nullptr;

		LoadToVulkan();
	}

	Model::~Model()
	{
		/*vkDeviceWaitIdle(Display::Get()->getVkDevice());
		vkFreeMemory(Display::Get()->getVkDevice(), m_memory, nullptr);
		vkDestroyBuffer(Display::Get()->getVkDevice(), m_indicesBuffer, nullptr);
		vkDestroyBuffer(Display::Get()->getVkDevice(), m_verticesBuffer, nullptr);
		vkDestroyBuffer(Display::Get()->getVkDevice(), m_texturesBuffer, nullptr);
		vkDestroyBuffer(Display::Get()->getVkDevice(), m_normalsBuffer, nullptr);
		vkDestroyBuffer(Display::Get()->getVkDevice(), m_tangentsBuffer, nullptr);*/

		delete m_vertices;
		delete m_textures;
		delete m_normals;
		delete m_tangents;
		delete m_indices;

		delete m_aabb;
	}

	void Model::LoadFromFile()
	{
		std::string fileLoaded = HelperFile::ReadTextFile(std::string(m_file));
		std::vector<std::string> lines = HelperString::Split(fileLoaded, "\n");

		std::vector<int> indices = std::vector<int>();
		std::vector<material> materials = std::vector<material>();
		std::vector<vertexdata*> vertices = std::vector<vertexdata*>();
		std::vector<Vector2> textures = std::vector<Vector2>();
		std::vector<Vector3> normals = std::vector<Vector3>();

		std::vector<std::string> splitFile = HelperString::Split(std::string(m_file), "/");
		std::string fileName = splitFile.at(splitFile.size() - 1);

		material currentMaterial = {};

		for (auto it = lines.begin(); it < lines.end(); ++it)
		{
			std::string line = HelperString::Trim(*it);

			std::vector<std::string> split = HelperString::Split(line, " ");

			if (!split.empty())
			{
				std::string prefix = split.at(0);

				if (prefix == "#")
				{
					continue;
				}

				if (prefix == "mtllib")
				{
					std::string pathMtl = HelperString::Substring(m_file, 0, (int) (m_file.length() - fileName.length()));
					pathMtl += split.at(1);
					LoadMaterials(pathMtl, &materials);
				}
				else if (prefix == "usemtl")
				{
					for (auto m : materials)
					{
						if (m.name == split.at(1))
						{
							currentMaterial = m;
						}
					}
				}
				else if (prefix == "v")
				{
					Vector3 vertex = Vector3(stof(split.at(1)), stof(split.at(2)), stof(split.at(3)));
					vertexdata *newVertex = new vertexdata((int) vertices.size(), vertex);
					vertices.push_back(newVertex);
				}
				else if (prefix == "vt")
				{
					Vector2 texture = Vector2(stof(split.at(1)), stof(split.at(2)));
					textures.push_back(texture);
				}
				else if (prefix == "vn")
				{
					Vector3 normal = Vector3(stof(split.at(1)), stof(split.at(2)), stof(split.at(3)));
					normals.push_back(normal);
				}
				else if (prefix == "f")
				{
					// The split length of 3 faced + 1 for the f prefix.
					if (split.size() != 4 || HelperString::Contains(line, "//"))
					{
						printf("Error reading the OBJ '%s', it does not appear to be UV mapped! The model will not be loaded.\n", m_file.c_str());
						//throw ex
					}

					std::vector<std::string> vertex1 = HelperString::Split(split.at(1), "/");
					std::vector<std::string> vertex2 = HelperString::Split(split.at(2), "/");
					std::vector<std::string> vertex3 = HelperString::Split(split.at(3), "/");

					vertexdata *v0 = ProcessDataVertex(Vector3(stof(vertex1.at(0)), stof(vertex1.at(1)), stof(vertex1.at(2))), &vertices, &indices);
					vertexdata *v1 = ProcessDataVertex(Vector3(stof(vertex2.at(0)), stof(vertex2.at(1)), stof(vertex2.at(2))), &vertices, &indices);
					vertexdata *v2 = ProcessDataVertex(Vector3(stof(vertex3.at(0)), stof(vertex3.at(1)), stof(vertex3.at(2))), &vertices, &indices);
					CalculateTangents(v0, v1, v2, &textures);
				}
				else if (prefix == "o")
				{
				}
				else if (prefix == "s")
				{
					// Smooth faces?
				}
				else
				{
					printf("OBJ '%s' ", m_file.c_str());
					printf("unknown line: '%s'.\n", line.c_str());
				}
			}
		}

		// Averages out vertex tangents, and disabled non set vertices,
		for (auto vertex : vertices)
		{
			vertex->averageTangents();

			if (!vertex->isSet())
			{
				vertex->setTextureIndex(0);
				vertex->setNormalIndex(0);
			}
		}

		// Turns the loaded OBJ data into a formal that can be used by OpenGL.
		m_indices = new std::vector<int>();
		m_vertices = new std::vector<float>();
		m_textures = new std::vector<float>();
		m_normals = new std::vector<float>();
		m_tangents = new std::vector<float>();

		m_indices->swap(indices);

		for (auto currentVertex : vertices)
		{
			Vector3 position = currentVertex->getPosition();
			Vector2 textureCoord = textures.at(currentVertex->getTextureIndex());
			Vector3 normalVector = normals.at(currentVertex->getNormalIndex());
			Vector3 tangent = currentVertex->getAverageTangent();

			m_vertices->push_back(position.m_x);
			m_vertices->push_back(position.m_y);
			m_vertices->push_back(position.m_z);

			m_textures->push_back(textureCoord.m_x);
			m_textures->push_back(1.0f - textureCoord.m_y);

			m_normals->push_back(normalVector.m_x);
			m_normals->push_back(normalVector.m_y);
			m_normals->push_back(normalVector.m_z);

			m_tangents->push_back(tangent.m_x);
			m_tangents->push_back(tangent.m_y);
			m_tangents->push_back(tangent.m_z);

			delete currentVertex;
		}
	}

	void Model::LoadMaterials(const std::string &filepath, std::vector<material> *list)
	{
		std::string fileLoaded = HelperFile::ReadTextFile(filepath);
		std::vector<std::string> lines = HelperString::Split(fileLoaded, "\n");

		std::string parseMaterialName = "";
		material parseMaterial = {};

		for (auto it = lines.begin(); it < lines.end(); ++it)
		{
			std::string line = HelperString::Trim(*it);

			std::vector<std::string> split = HelperString::Split(line, " ");

			if (!split.empty())
			{
				std::string prefix = split.at(0);

				if (prefix == "#")
				{
					continue;
				}

				if (prefix == "newmtl")
				{
					if (parseMaterialName != "")
					{
						list->push_back(parseMaterial);
					}
				}
				else if (prefix == "Ns")
				{
				}
				else if (prefix == "Ka")
				{
				}
				else if (prefix == "Kd")
				{
				}
				else if (prefix == "Ks")
				{
				}
				else if (prefix == "map_Kd")
				{
				}
				else if (prefix == "map_Bump")
				{
				}
			}
			else
			{
				printf("MTL '%s' ", filepath.c_str());
				printf("unknown line: '%s'.\n", line.c_str());
			}
		}
	}

	vertexdata *Model::ProcessDataVertex(Vector3 vertex, std::vector<vertexdata*> *vertices, std::vector<int> *indices)
	{
		int index = static_cast<int>(vertex.m_x) - 1;
		vertexdata *currentVertex = vertices->at(index);
		int textureIndex = static_cast<int>(vertex.m_y) - 1;
		int normalIndex = static_cast<int>(vertex.m_z) - 1;

		if (!currentVertex->isSet())
		{
			currentVertex->setTextureIndex(textureIndex);
			currentVertex->setNormalIndex(normalIndex);
			indices->push_back(index);
			return currentVertex;
		}
		return DealWithAlreadyProcessedDataVertex(currentVertex, textureIndex, normalIndex, indices, vertices);
	}

	vertexdata *Model::DealWithAlreadyProcessedDataVertex(vertexdata *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<int> *indices, std::vector<vertexdata*> *vertices)
	{
		if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex))
		{
			indices->push_back(previousVertex->getIndex());
			return previousVertex;
		}
		vertexdata *anotherVertex = previousVertex->getDuplicateVertex();

		if (anotherVertex != nullptr)
		{
			return DealWithAlreadyProcessedDataVertex(anotherVertex, newTextureIndex, newNormalIndex, indices, vertices);
		}
		vertexdata *duplicateVertex = new vertexdata((int) vertices->size(), previousVertex->getPosition());
		duplicateVertex->setTextureIndex(newTextureIndex);
		duplicateVertex->setNormalIndex(newNormalIndex);
		previousVertex->setDuplicateVertex(duplicateVertex);
		vertices->push_back(duplicateVertex);
		indices->push_back(duplicateVertex->getIndex());
		return duplicateVertex;
	}

	void Model::CalculateTangents(vertexdata *v0, vertexdata *v1, vertexdata *v2, std::vector<Vector2> *textures)
	{
		Vector3 *deltaPos1 = Vector3::subtract(v1->getPosition(), v0->getPosition(), nullptr);
		Vector3 *deltaPos2 = Vector3::subtract(v2->getPosition(), v0->getPosition(), nullptr);
		Vector2 uv0 = textures->at(v0->getTextureIndex());
		Vector2 uv1 = textures->at(v1->getTextureIndex());
		Vector2 uv2 = textures->at(v2->getTextureIndex());
		Vector2 *deltaUv1 = Vector2::subtract(uv1, uv0, nullptr);
		Vector2 *deltaUv2 = Vector2::subtract(uv2, uv0, nullptr);

		float r = 1.0f / (deltaUv1->m_x * deltaUv2->m_y - deltaUv1->m_y * deltaUv2->m_x);
		deltaPos1->scale(deltaUv2->m_y);
		deltaPos2->scale(deltaUv1->m_y);

		Vector3 *tangent = Vector3::subtract(*deltaPos1, *deltaPos2, nullptr);
		tangent->scale(r);
		v0->addTangent(tangent);
		v1->addTangent(tangent);
		v2->addTangent(tangent);

		delete deltaPos1;
		delete deltaPos2;
		delete deltaUv1;
		delete deltaUv2;
	}

	void Model::LoadToVulkan()
	{
		/*if (m_indices != nullptr && !m_indices->empty())
		{
		//	m_indicesBuffer = createIndicesBuffer(m_indices);
		}

		if (m_vertices != nullptr && !m_vertices->empty())
		{
			m_verticesBuffer = createBuffer(m_vertices);
		}

		if (m_textures != nullptr && !m_textures->empty())
		{
			m_texturesBuffer = createBuffer(m_textures);
		}

		if (m_normals != nullptr && !m_normals->empty())
		{
			m_normalsBuffer = createBuffer(m_normals);
		}

		if (m_tangents != nullptr && !m_tangents->empty())
		{
			m_tangentsBuffer = createBuffer(m_tangents);
		}*/
	}

	VkBuffer Model::CreateBuffer(std::vector<float> *data)
	{
		VkBuffer result = VK_NULL_HANDLE;

		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferCreateInfo.size = data->size();
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		Display::vkErrorCheck(vkCreateBuffer(Display::Get()->GetVkDevice(), &bufferCreateInfo, nullptr, &result));

		VkMemoryRequirements memoryRequirements = {};
		vkGetBufferMemoryRequirements(Display::Get()->GetVkDevice(), result, &memoryRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		MemoryTypeFromProperties(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &allocInfo.memoryTypeIndex);

		Display::vkErrorCheck(vkAllocateMemory(Display::Get()->GetVkDevice(), &allocInfo, nullptr, &m_memory));

		m_bufferInfo.range = memoryRequirements.size;
		m_bufferInfo.offset = 0;

		uint8_t *pData;
		Display::vkErrorCheck(vkMapMemory(Display::Get()->GetVkDevice(), m_memory, 0, memoryRequirements.size, 0, (void**) &pData));

		memcpy(pData, data->data(), data->size());

		vkUnmapMemory(Display::Get()->GetVkDevice(), m_memory);

		Display::vkErrorCheck(vkBindBufferMemory(Display::Get()->GetVkDevice(), result, m_memory, 0));

		return result;
	}

	void Model::MemoryTypeFromProperties(uint32_t typeBits, VkFlags reqMask, uint32_t *typeIndex)
	{
		for (uint32_t i = 0; i < Display::Get()->GetVkPhysicalDeviceMemoryProperties().memoryTypeCount; i++)
		{
			if ((typeBits & 1) == 1)
			{
				if ((Display::Get()->GetVkPhysicalDeviceMemoryProperties().memoryTypes[i].propertyFlags & reqMask) == reqMask)
				{
					*typeIndex = i;
					return;
				}
			}

			typeBits >>= 1;
		}
	}

	void Model::CreateAabb()
	{
		if (m_aabb == nullptr)
		{
			m_aabb = new aabb();
		}

		float minX = +INFINITY;
		float minY = +INFINITY;
		float minZ = +INFINITY;
		float maxX = -INFINITY;
		float maxY = -INFINITY;
		float maxZ = -INFINITY;

		if (m_vertices->size() > 1)
		{
			for (int i = 0; i < m_vertices->size(); i += 3)
			{
				float x = m_vertices->at(i);
				float y = m_vertices->at(i + 1);
				float z = m_vertices->at(i + 2);

				if (x < minX)
				{
					minX = x;
				}
				else if (x > maxX)
				{
					maxX = x;
				}

				if (y < minY)
				{
					minY = y;
				}
				else if (y > maxY)
				{
					maxY = y;
				}

				if (z < minZ)
				{
					minZ = z;
				}
				else if (z > maxZ)
				{
					maxZ = z;
				}
			}
		}

		//m_aabb->extentsMin->set(minX, minY, minZ);
		//m_aabb->extentsMax->set(maxX, maxY, maxZ);
	}
}
