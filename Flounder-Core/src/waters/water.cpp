#include "water.h"

namespace flounder
{
	const float water::WAVE_SPEED = 15.0f;
	const float water::WAVE_LENGTH = 30.0f;
	const float water::AMPLITUDE = 0.7f;

	const float water::SQUARE_SIZE = 2.598f;
	const int water::VERTEX_COUNT = 176;

	const colour water::WATER_COLOUR = colour(0.21f, 0.41f, 0.59f);

	const float water::SHINE_DAMPER = 1.0f;
	const float water::REFLECTIVITY = 0.0f;

	water::water(const vector3 &position, const vector3 &rotation)
	{
		m_model = 0;

		m_colour = new colour(WATER_COLOUR);

		m_position = new vector3(position);
		m_rotation = new vector3(rotation);
		m_offset = new vector3();
		m_moved = true;

		m_modelMatrix = new matrix4x4();
		m_aabb = new aabb();

		generateMesh();
	}

	water::~water()
	{
		delete m_model;

		delete m_colour;

		delete m_position;
		delete m_rotation;
		delete m_offset;

		delete m_modelMatrix;
		delete m_aabb;
	}

	void water::update()
	{
		if (m_moved)
		{
			matrix4x4::transformationMatrix(*m_position, *m_rotation, 1.0f, m_modelMatrix);
			m_moved = false;
		}

		m_offset->set(
			2.0f * water::SQUARE_SIZE * round(camera::get()->getCamera()->getPosition()->m_x / (2.0f * water::SQUARE_SIZE)),
			0.0f,
			2.0f * water::SQUARE_SIZE * round(camera::get()->getCamera()->getPosition()->m_z / (2.0f * water::SQUARE_SIZE))
		);
	}

	void water::generateMesh()
	{
		std::vector<float> *vertices = new std::vector<float>();

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
					storeQuad1(vertices, topLeft, topRight, bottomLeft, bottomRight, col % 2 == 0);
				}
				else
				{
					storeQuad2(vertices, topLeft, topRight, bottomLeft, bottomRight, col % 2 == 0);
				}
			}
		}

		m_model = new model(nullptr, vertices, nullptr, nullptr, nullptr);

		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_aabb->update(*m_position, *m_rotation, 1.0f, m_aabb);

		//	delete vertices;
	}

	void water::storeQuad1(std::vector<float> *vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed)
	{
		storeVertex(vertices, topLeft, vector2(0.0f, 1.0f), mixed ? vector2(1.0f, 0.0f) : vector2(1.0f, 1.0f));
		storeVertex(vertices, bottomLeft, mixed ? vector2(1.0f, -1.0f) : vector2(1.0f, 0.0f), vector2(0.0f, -1.0f));

		if (mixed)
		{
			storeVertex(vertices, topRight, vector2(-1.0f, 0.0f), vector2(-1.0f, 1.0f));
		}
		else
		{
			storeVertex(vertices, bottomRight, vector2(-1.0f, -1.0f), vector2(-1.0f, 0.0f));
		}

		storeVertex(vertices, bottomRight, vector2(0.0f, -1.0f), mixed ? vector2(-1.0f, 0.0f) : vector2(-1.0f, -1.0f));
		storeVertex(vertices, topRight, mixed ? vector2(-1.0f, 1.0f) : vector2(-1.0f, 0.0f), vector2(0.0f, 1.0f));

		if (mixed)
		{
			storeVertex(vertices, bottomLeft, vector2(1.0f, 0.0f), vector2(1.0f, -1.0f));
		}
		else
		{
			storeVertex(vertices, topLeft, vector2(1.0f, 1.0f), vector2(1.0f, 0.0f));
		}
	}

	void water::storeQuad2(std::vector<float> *vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed)
	{
		storeVertex(vertices, topRight, vector2(-1.0f, 0.0f), mixed ? vector2(0.0f, 1.0f) : vector2(-1.0f, 1.0f));
		storeVertex(vertices, topLeft, mixed ? vector2(1.0f, 1.0f) : vector2(0.0f, 1.0f), vector2(1.0f, 0.0f));

		if (mixed)
		{
			storeVertex(vertices, bottomRight, vector2(0.0f, -1.0f), vector2(-1.0f, -1.0f));
		}
		else
		{
			storeVertex(vertices, bottomLeft, vector2(1.0f, -1.0f), vector2(0.0f, -1.0f));
		}

		storeVertex(vertices, bottomLeft, vector2(1.0f, 0.0f), mixed ? vector2(0.0f, -1.0f) : vector2(1.0f, -1.0f));
		storeVertex(vertices, bottomRight, mixed ? vector2(-1.0f, -1.0f) : vector2(0.0f, -1.0f), vector2(-1.0f, 0.0f));

		if (mixed)
		{
			storeVertex(vertices, topLeft, vector2(0.0f, 1.0f), vector2(1.0f, 1.0f));
		}
		else
		{
			storeVertex(vertices, topRight, vector2(-1.0f, 1.0f), vector2(0.0f, 1.0f));
		}
	}

	void water::storeVertex(std::vector<float> *vertices, const int &index, const vector2 &otherPoint1, const vector2 &otherPoint2)
	{
		int gridX = index % VERTEX_COUNT;
		int gridZ = index / VERTEX_COUNT;
		double x = gridX * SQUARE_SIZE;
		double z = gridZ * SQUARE_SIZE;

		if (x > m_aabb->m_maxExtents->m_x)
		{
			m_aabb->m_maxExtents->m_x = static_cast<float>(x);
		}
		else if (x < m_aabb->m_minExtents->m_x)
		{
			m_aabb->m_minExtents->m_x = static_cast<float>(x);
		}

		if (z > m_aabb->m_maxExtents->m_z)
		{
			m_aabb->m_maxExtents->m_z = static_cast<float>(z);
		}
		else if (z < m_aabb->m_minExtents->m_z)
		{
			m_aabb->m_minExtents->m_z = static_cast<float>(z);
		}

		vertices->push_back(static_cast<float>(x));
		vertices->push_back(static_cast<float>(z));
		vertices->push_back(encode(otherPoint1.m_x, otherPoint1.m_y, otherPoint2.m_x, otherPoint2.m_y));
	}

	float water::encode(const float &x, const float &z, const float &x2, const float &z2)
	{
		float p3 = (x + 1.0f) * 27.0f;
		float p2 = (z + 1.0f) * 9.0f;
		float p1 = (x2 + 1.0f) * 3.0f;
		float p0 = (z2 + 1.0f) * 1.0f;
		return p0 + p1 + p2 + p3;
	}

	float water::getHeight(const float &x, const float &z)
	{
		float waveTime = framework::get()->getTimeSec() / WAVE_SPEED;

		const double val1 = 0.1;
		const double val2 = 0.3;
		double radiansX = 2.0 * PI * ((maths::mod(x + z * x * val1, static_cast<double>(WAVE_LENGTH)) / static_cast<double>(WAVE_LENGTH)) + waveTime);
		double radiansZ = 2.0 * PI * ((maths::mod(val2 * (z * x + x * z), static_cast<double>(WAVE_LENGTH)) / static_cast<double>(WAVE_LENGTH)) + waveTime * 2.0);
		double result = 0.5 * AMPLITUDE * (cos(radiansZ + sin(x)) + sin(radiansX - cos(z)));
		return static_cast<float>(m_position->m_y + result);
	}

	void water::setPosition(const vector3 &position)
	{
		m_position->set(position);
		m_moved = true;
	}

	void water::setRotation(const vector3 &rotation)
	{
		m_rotation->set(rotation);
		m_moved = true;
	}
}
