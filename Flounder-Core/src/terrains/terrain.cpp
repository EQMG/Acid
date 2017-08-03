#include "terrain.h"

namespace flounder
{
	const double terrain::SQUARE_SIZE = 2.598f;
	const int terrain::VERTEX_COUNT = 176;

	terrain::terrain(const vector3 &position, const vector3 &rotation)
	{
		m_vaoID = 0;
		m_vaoLength = 0;

		m_position = new vector3(position);
		m_rotation = new vector3(rotation);
		m_moved = true;

		m_modelMatrix = new matrix4x4();
		m_aabb = new aabb();

		generateMesh();
	}

	terrain::~terrain()
	{
		glDeleteVertexArrays(1, &m_vaoID);

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
		std::vector<GLfloat> vertices = std::vector<GLfloat>();
		std::vector<GLfloat> normals = std::vector<GLfloat>();
		std::vector<GLfloat> textures = std::vector<GLfloat>();
		std::vector<GLint> indices = std::vector<GLint>(6 * (VERTEX_COUNT - 1) * VERTEX_COUNT);

		for (int col = 0; col < VERTEX_COUNT; col++)
		{
			for (int row = 0; row < VERTEX_COUNT; row++)
			{
				vector3 position = calculatePosition(row, col, noise);
				vector3 normal = calculateNormal(row, col, noise);

				vertices.push_back(position.m_x);
				vertices.push_back(position.m_y);
				vertices.push_back(position.m_z);

				normals.push_back(normal.m_x);
				normals.push_back(normal.m_y);
				normals.push_back(normal.m_z);

				textures.push_back(static_cast<float>(row) / (static_cast<float>(VERTEX_COUNT) - 1));
				textures.push_back(static_cast<float>(col) / (static_cast<float>(VERTEX_COUNT) - 1));
			}
		}

		for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
		{
			for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
			{
				int topLeft = (gz * VERTEX_COUNT) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
				int bottomRight = bottomLeft + 1;
				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}

		m_vaoID = loaders::get()->createVAO();
		loaders::get()->createIndicesVBO(m_vaoID, indices);
		loaders::get()->storeDataInVBO(m_vaoID, vertices, 0, 3);
		loaders::get()->storeDataInVBO(m_vaoID, textures, 1, 2);
		loaders::get()->storeDataInVBO(m_vaoID, normals, 2, 3);
		loaders::get()->unbindVAO();
		m_vaoLength = indices.size();
	}

	vector3 terrain::calculateNormal(const int & x, const int & z, noisesimplex & noise)
	{
		vector3 positionL = calculatePosition(x - 1, z, noise);
		vector3 positionR = calculatePosition(x + 1, z, noise);
		vector3 positionD = calculatePosition(x, z - 1, noise);
		vector3 positionU = calculatePosition(x, z + 1, noise);

		vector3 normal = vector3(positionL.m_y - positionR.m_y, 2.0f, positionD.m_y - positionU.m_y);
		normal.normalize();
		return normal;
	}

	vector3 terrain::calculatePosition(const int & x, const int & z, noisesimplex & noise)
	{
		float worldX = (static_cast<float>(x) * SQUARE_SIZE) - ((SQUARE_SIZE * VERTEX_COUNT) / 2.0f);
		float worldZ = (static_cast<float>(z) * SQUARE_SIZE) - ((SQUARE_SIZE * VERTEX_COUNT) / 2.0f);
		float worldY = 10.0f * noise.noise(worldX / 100.0f, worldZ / 100.0f) + 4.20f;
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
