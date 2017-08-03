#include "terrain.h"

namespace flounder
{
	const double terrain::SQUARE_SIZE = 2.598f;
	const int terrain::VERTEX_COUNT = 176;

	terrain::terrain(const vector3 &position, const vector3 &rotation)
	{
		m_model = NULL;

		m_position = new vector3(position);
		m_rotation = new vector3(rotation);
		m_moved = true;

		m_modelMatrix = new matrix4x4();
		m_aabb = new aabb();

		generateMesh();
	}

	terrain::~terrain()
	{
		delete m_model;

		delete m_position;
		delete m_rotation;

		delete m_modelMatrix;
		delete m_aabb;
	}

	void terrain::update()
	{
		if (m_moved)
		{
			matrix4x4::transformationMatrix(*m_position, *m_rotation, 1.0f, m_modelMatrix);
			m_moved = false;
		}
	}

	void terrain::generateMesh()
	{
		int count = VERTEX_COUNT * VERTEX_COUNT;
		noisesimplex noise = noisesimplex(0420);
		std::vector<GLfloat> *vertices = new std::vector<GLfloat>();
		std::vector<GLfloat> *normals = new std::vector<GLfloat>();
		std::vector<GLfloat> *colours = new std::vector<GLfloat>();
		std::vector<GLint> *indices = new std::vector<GLint>();

		const colour colourDirt = colour(120, 72, 0, true);
		const colour colourGrass = colour(124, 252, 0, true);
		const colour colourStone = colour(169, 169, 169, true);
		const colour colourSnow = colour(235, 235, 245, true);
		const colour colourSand = colour(194, 178, 128, true);

		for (int col = 0; col < VERTEX_COUNT; col++)
		{
			for (int row = 0; row < VERTEX_COUNT; row++)
			{
				vector3 position = calculatePosition(row, col, noise);
				vector3 normal = calculateNormal(row, col, noise);
				
				colour *tint = colour::interpolate(colourDirt, colourGrass, std::min(fabs(normal.m_y), 1.0f), NULL);

				if (position.m_y - 20.0f > 0.0f)
				{
					colour::interpolate(*tint, colourSnow, std::min(fabs(position.m_y - 20.0f), 1.0f), tint);
				}
				else if (position.m_y - 17.0f > 0.0f)
				{
					colour::interpolate(*tint, colourStone, std::min(fabs(position.m_y - 17.0f), 1.0f), tint);
				}
				else if (position.m_y < 2.0f)
				{
					colour::interpolate(*tint, colourSand, std::min(fabs(position.m_y), 1.0f), tint);
				}

				storeVertex(vertices, position);

				normals->push_back(normal.m_x);
				normals->push_back(normal.m_y);
				normals->push_back(normal.m_z);

				colours->push_back(tint->m_r);
				colours->push_back(tint->m_g);
				colours->push_back(tint->m_b);

				delete tint;
			}
		}

		for (int col = 0; col < VERTEX_COUNT - 1; col++)
		{
			for (int row = 0; row < VERTEX_COUNT - 1; row++)
			{
				int topLeft = (row * VERTEX_COUNT) + col;
				int topRight = topLeft + 1;
				int bottomLeft = ((row + 1) * VERTEX_COUNT) + col;
				int bottomRight = bottomLeft + 1;

				if (row % 2 == 0)
				{
					storeQuad1(indices, topLeft, topRight, bottomLeft, bottomRight, col % 2 == 0);
				}
				else
				{
					storeQuad2(indices, topLeft, topRight, bottomLeft, bottomRight, col % 2 == 0);
				}
			}
		}

		m_model = model::newModel()->setDirectly(indices, vertices, NULL, normals, colours)->create();

		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_aabb->update(*m_position, *m_rotation, 1.0f, m_aabb);

		delete vertices;
		delete normals;
		delete colours;
		delete indices;
	}

	void terrain::storeVertex(std::vector<GLfloat> *vertices, const vector3 & vertex)
	{
		if (vertex.m_x > m_aabb->m_maxExtents->m_x)
		{
			m_aabb->m_maxExtents->m_x = vertex.m_x;
		}
		else if (vertex.m_x < m_aabb->m_minExtents->m_x)
		{
			m_aabb->m_minExtents->m_x = vertex.m_x;
		}

		if (vertex.m_y > m_aabb->m_maxExtents->m_y)
		{
			m_aabb->m_maxExtents->m_y = vertex.m_y;
		}
		else if (vertex.m_y < m_aabb->m_minExtents->m_y)
		{
			m_aabb->m_minExtents->m_y = vertex.m_z;
		}

		if (vertex.m_z > m_aabb->m_maxExtents->m_z)
		{
			m_aabb->m_maxExtents->m_z = vertex.m_z;
		}
		else if (vertex.m_z < m_aabb->m_minExtents->m_z)
		{
			m_aabb->m_minExtents->m_z = vertex.m_z;
		}

		vertices->push_back(vertex.m_x);
		vertices->push_back(vertex.m_y);
		vertices->push_back(vertex.m_z);
	}

	void terrain::storeQuad1(std::vector<GLint> *indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed)
	{
		indices->push_back(topLeft);
		indices->push_back(bottomLeft);
		indices->push_back(mixed ? topRight : bottomRight);
		indices->push_back(bottomRight);
		indices->push_back(topRight);
		indices->push_back(mixed ? bottomLeft : topLeft);
	}

	void terrain::storeQuad2(std::vector<GLint> *indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed)
	{
		indices->push_back(topRight);
		indices->push_back(topLeft);
		indices->push_back(mixed ? bottomRight : bottomLeft);
		indices->push_back(bottomLeft);
		indices->push_back(bottomRight);
		indices->push_back(mixed ? topLeft : topRight);
	}

	vector3 terrain::calculateNormal(const int &x, const int &z, noisesimplex &noise)
	{
		vector3 positionL = calculatePosition(x - 1, z, noise);
		vector3 positionR = calculatePosition(x + 1, z, noise);
		vector3 positionD = calculatePosition(x, z - 1, noise);
		vector3 positionU = calculatePosition(x, z + 1, noise);

		vector3 normal = vector3(positionL.m_y - positionR.m_y, 2.0f, positionD.m_y - positionU.m_y);
		normal.normalize();
		return normal;
	}

	vector3 terrain::calculatePosition(const int &x, const int &z, noisesimplex &noise)
	{
		float worldX = static_cast<float>(x) * SQUARE_SIZE;
		float worldZ = static_cast<float>(z) * SQUARE_SIZE;
		float worldY = 25.0f * noise.noise(worldX / 115.0f, worldZ / 115.0f) + 8.0f;
		return vector3(worldX, worldY, worldZ);
	}

	void terrain::setPosition(const vector3 &position)
	{
		m_position->set(position);
		m_moved = true;
	}

	void terrain::setRotation(const vector3 &rotation)
	{
		m_rotation->set(rotation);
		m_moved = true;
	}
}
