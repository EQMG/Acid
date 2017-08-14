#include "model.h"

namespace flounder
{
	model::model(const std::string &file)
	{
		m_file = file;

		m_indices = nullptr;
		m_vertices = nullptr;
		m_textures = nullptr;
		m_normals = nullptr;
		m_tangents = nullptr;

		m_aabb = nullptr;

#if 0
		m_vaoID = 0;
		m_vaoLength = 0;
#endif

		loadFromFile();
	}

	model::model(std::vector<int> *indices, std::vector<float> *vertices, std::vector<float> *textures, std::vector<float> *normals, std::vector<float> *tangents)
	{
		m_file = "";

		m_indices = indices;
		m_vertices = vertices;
		m_textures = textures;
		m_normals = normals;
		m_tangents = tangents;

		m_aabb = nullptr;

#if 0
		m_vaoID = 0;
		m_vaoLength = 0;
#endif

		loadToOpenGL();
	}

	model::~model()
	{
#if 0
		glDeleteVertexArrays(1, &m_vaoID);
#endif

		/*delete m_vertices;
		delete m_textures;
		delete m_normals;
		delete m_tangents;
		delete m_indices;*/

		delete m_aabb;
	}

	void model::loadFromFile()
	{
		std::string fileLoaded = helperfile::readTextFile(m_file);
		std::vector<std::string> lines = helperstring::split(fileLoaded, "\n");

		std::vector<int> indices = std::vector<int>();
		std::vector<vertexdata*> vertices = std::vector<vertexdata*>();
		std::vector<vector2> textures = std::vector<vector2>();
		std::vector<vector3> normals = std::vector<vector3>();

		for (std::vector<std::string>::iterator it = lines.begin(); it < lines.end(); it++)
		{
			std::string line = helperstring::trim(*it);

			std::vector<std::string> split = helperstring::split(line, " ");

			if (!split.empty())
			{
				std::string prefix = split.at(0);

				if (prefix == "#")
				{
					continue;
				}

				if (prefix == "v")
				{
					vector3 vertex = vector3(stof(split.at(1)), stof(split.at(2)), stof(split.at(3)));
					vertexdata *newVertex = new vertexdata(vertices.size(), vertex);
					vertices.push_back(newVertex);
				}
				else if (prefix == "vt")
				{
					vector2 texture = vector2(stof(split.at(1)), stof(split.at(2)));
					textures.push_back(texture);
				}
				else if (prefix == "vn")
				{
					vector3 normal = vector3(stof(split.at(1)), stof(split.at(2)), stof(split.at(3)));
					normals.push_back(normal);
				}
				else if (prefix == "f")
				{
					// The split length of 3 faced + 1 for the f prefix.
					if (split.size() != 4 || helperstring::contains(line, "//"))
					{
						std::cout << "Error reading the OBJ " << m_file << ", it does not appear to be UV mapped! The model will not be loaded." << std::endl;
						//throw ex
					}

					std::vector<std::string> vertex1 = helperstring::split(split.at(1), "/");
					std::vector<std::string> vertex2 = helperstring::split(split.at(2), "/");
					std::vector<std::string> vertex3 = helperstring::split(split.at(3), "/");

					vertexdata *v0 = processDataVertex(vector3(stoi(vertex1.at(0)), stoi(vertex1.at(1)), stoi(vertex1.at(2))), &vertices, &indices);
					vertexdata *v1 = processDataVertex(vector3(stoi(vertex2.at(0)), stoi(vertex2.at(1)), stoi(vertex2.at(2))), &vertices, &indices);
					vertexdata *v2 = processDataVertex(vector3(stoi(vertex3.at(0)), stoi(vertex3.at(1)), stoi(vertex3.at(2))), &vertices, &indices);
					calculateTangents(v0, v1, v2, &textures);
				}
				else
				{
					std::cout << "OBJ " << m_file + " unknown line: " << line << std::endl;
				}
			}
		}

		// Averages out vertex tangents, and disabled non set vertices,
		for (vertexdata *vertex : vertices)
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

		for (vertexdata *currentVertex : vertices)
		{
			vector3 position = currentVertex->getPosition();
			vector2 textureCoord = textures.at(currentVertex->getTextureIndex());
			vector3 normalVector = normals.at(currentVertex->getNormalIndex());
			vector3 tangent = currentVertex->getAverageTangent();

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

	vertexdata *model::processDataVertex(vector3 vertex, std::vector<vertexdata*> *vertices, std::vector<int> *indices)
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
		else
		{
			return dealWithAlreadyProcessedDataVertex(currentVertex, textureIndex, normalIndex, indices, vertices);
		}
	}

	vertexdata *model::dealWithAlreadyProcessedDataVertex(vertexdata *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<int> *indices, std::vector<vertexdata*> *vertices)
	{
		if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex))
		{
			indices->push_back(previousVertex->getIndex());
			return previousVertex;
		}
		else
		{
			vertexdata *anotherVertex = previousVertex->getDuplicateVertex();

			if (anotherVertex != nullptr)
			{
				return dealWithAlreadyProcessedDataVertex(anotherVertex, newTextureIndex, newNormalIndex, indices, vertices);
			}
			else
			{
				vertexdata *duplicateVertex = new vertexdata(vertices->size(), previousVertex->getPosition());
				duplicateVertex->setTextureIndex(newTextureIndex);
				duplicateVertex->setNormalIndex(newNormalIndex);
				previousVertex->setDuplicateVertex(duplicateVertex);
				vertices->push_back(duplicateVertex);
				indices->push_back(duplicateVertex->getIndex());
				return duplicateVertex;
			}
		}
	}

	void model::calculateTangents(vertexdata *v0, vertexdata *v1, vertexdata *v2, std::vector<vector2> *textures)
	{
		vector3 *deltaPos1 = vector3::subtract(v1->getPosition(), v0->getPosition(), nullptr);
		vector3 *deltaPos2 = vector3::subtract(v2->getPosition(), v0->getPosition(), nullptr);
		vector2 uv0 = textures->at(v0->getTextureIndex());
		vector2 uv1 = textures->at(v1->getTextureIndex());
		vector2 uv2 = textures->at(v2->getTextureIndex());
		vector2 *deltaUv1 = vector2::subtract(uv1, uv0, nullptr);
		vector2 *deltaUv2 = vector2::subtract(uv2, uv0, nullptr);

		float r = 1.0f / (deltaUv1->m_x * deltaUv2->m_y - deltaUv1->m_y * deltaUv2->m_x);
		deltaPos1->scale(deltaUv2->m_y);
		deltaPos2->scale(deltaUv1->m_y);

		vector3 *tangent = vector3::subtract(*deltaPos1, *deltaPos2, nullptr);
		tangent->scale(r);
		v0->addTangent(tangent);
		v1->addTangent(tangent);
		v2->addTangent(tangent);

		delete deltaPos1;
		delete deltaPos2;
		delete deltaUv1;
		delete deltaUv2;
	}

	void model::loadToOpenGL()
	{
#if 0
		m_vaoID = loaders::get()->createVAO();
		
		if (m_indices != nullptr)
		{
			loaders::get()->createIndicesVBO(m_vaoID, *m_indices);
		}

		if (m_vertices != nullptr)
		{
			loaders::get()->storeDataInVBO(m_vaoID, *m_vertices, 0, 3);
		}

		if (m_textures != nullptr)
		{
			loaders::get()->storeDataInVBO(m_vaoID, *m_textures, 1, 2);
		}

		if (m_normals != nullptr)
		{
			loaders::get()->storeDataInVBO(m_vaoID, *m_normals, 2, 3);
		}

		if (m_tangents != nullptr)
		{
			loaders::get()->storeDataInVBO(m_vaoID, *m_tangents, 3, 3);
		}

		loaders::get()->unbindVAO();

		if (m_indices != nullptr)
		{
			m_vaoLength = m_indices->size();
		}
		else if (m_vertices != nullptr)
		{
			m_vaoLength = m_vertices->size() / 3;
		}
#endif
	}

	void model::createAABB()
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
