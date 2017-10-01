#include "Water.hpp"

#include "../camera/Camera.hpp"
#include "../maths/Maths.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	const float Water::WAVE_SPEED = 15.0f;
	const float Water::WAVE_LENGTH = 30.0f;
	const float Water::AMPLITUDE = 0.7f;

	const float Water::SQUARE_SIZE = 2.598f;
	const int Water::VERTEX_COUNT = 176;

	const Colour Water::WATER_COLOUR = Colour(0.21f, 0.41f, 0.59f, 1.0f);

	const float Water::SHINE_DAMPER = 1.0f;
	const float Water::REFLECTIVITY = 0.0f;

	Water::Water(const Vector3 &position, const Vector3 &rotation) :
		m_model(nullptr),
		m_colour(new Colour(WATER_COLOUR)),
		m_position(new Vector3(position)),
		m_rotation(new Vector3(rotation)),
		m_offset(new Vector3()),
		m_moved(true),
		m_modelMatrix(new Matrix4()),
		m_aabb(new Aabb())
	{
		GenerateMesh();
	}

	Water::~Water()
	{
		delete m_model;

		delete m_colour;

		delete m_position;
		delete m_rotation;
		delete m_offset;

		delete m_modelMatrix;
		delete m_aabb;
	}

	void Water::Update()
	{
		if (m_moved)
		{
			Matrix4::TransformationMatrix(*m_position, *m_rotation, 1.0f, m_modelMatrix);
			m_moved = false;
		}

		m_offset->Set(
			2.0f * SQUARE_SIZE * round(Camera::Get()->GetCamera()->GetPosition()->m_x / (2.0f * SQUARE_SIZE)),
			0.0f,
			2.0f * SQUARE_SIZE * round(Camera::Get()->GetCamera()->GetPosition()->m_z / (2.0f * SQUARE_SIZE))
		);
	}

	void Water::GenerateMesh()
	{
		std::vector<float> vertices = std::vector<float>();

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
					StoreQuad1(vertices, topLeft, topRight, bottomLeft, bottomRight, col % 2 == 0);
				}
				else
				{
					StoreQuad2(vertices, topLeft, topRight, bottomLeft, bottomRight, col % 2 == 0);
				}
			}
		}

		m_model = new Model(std::vector<int>(), vertices);

		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_aabb->Update(*m_position, *m_rotation, 1.0f, m_aabb);

		//	delete vertices;
	}

	void Water::StoreQuad1(std::vector<float> &vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed) const
	{
		StoreVertex(vertices, topLeft, Vector2(0.0f, 1.0f), mixed ? Vector2(1.0f, 0.0f) : Vector2(1.0f, 1.0f));
		StoreVertex(vertices, bottomLeft, mixed ? Vector2(1.0f, -1.0f) : Vector2(1.0f, 0.0f), Vector2(0.0f, -1.0f));

		if (mixed)
		{
			StoreVertex(vertices, topRight, Vector2(-1.0f, 0.0f), Vector2(-1.0f, 1.0f));
		}
		else
		{
			StoreVertex(vertices, bottomRight, Vector2(-1.0f, -1.0f), Vector2(-1.0f, 0.0f));
		}

		StoreVertex(vertices, bottomRight, Vector2(0.0f, -1.0f), mixed ? Vector2(-1.0f, 0.0f) : Vector2(-1.0f, -1.0f));
		StoreVertex(vertices, topRight, mixed ? Vector2(-1.0f, 1.0f) : Vector2(-1.0f, 0.0f), Vector2(0.0f, 1.0f));

		if (mixed)
		{
			StoreVertex(vertices, bottomLeft, Vector2(1.0f, 0.0f), Vector2(1.0f, -1.0f));
		}
		else
		{
			StoreVertex(vertices, topLeft, Vector2(1.0f, 1.0f), Vector2(1.0f, 0.0f));
		}
	}

	void Water::StoreQuad2(std::vector<float> &vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed) const
	{
		StoreVertex(vertices, topRight, Vector2(-1.0f, 0.0f), mixed ? Vector2(0.0f, 1.0f) : Vector2(-1.0f, 1.0f));
		StoreVertex(vertices, topLeft, mixed ? Vector2(1.0f, 1.0f) : Vector2(0.0f, 1.0f), Vector2(1.0f, 0.0f));

		if (mixed)
		{
			StoreVertex(vertices, bottomRight, Vector2(0.0f, -1.0f), Vector2(-1.0f, -1.0f));
		}
		else
		{
			StoreVertex(vertices, bottomLeft, Vector2(1.0f, -1.0f), Vector2(0.0f, -1.0f));
		}

		StoreVertex(vertices, bottomLeft, Vector2(1.0f, 0.0f), mixed ? Vector2(0.0f, -1.0f) : Vector2(1.0f, -1.0f));
		StoreVertex(vertices, bottomRight, mixed ? Vector2(-1.0f, -1.0f) : Vector2(0.0f, -1.0f), Vector2(-1.0f, 0.0f));

		if (mixed)
		{
			StoreVertex(vertices, topLeft, Vector2(0.0f, 1.0f), Vector2(1.0f, 1.0f));
		}
		else
		{
			StoreVertex(vertices, topRight, Vector2(-1.0f, 1.0f), Vector2(0.0f, 1.0f));
		}
	}

	void Water::StoreVertex(std::vector<float> &vertices, const int &index, const Vector2 &otherPoint1, const Vector2 &otherPoint2) const
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

		vertices.push_back(static_cast<float>(x));
		vertices.push_back(static_cast<float>(z));
		vertices.push_back(Encode(otherPoint1.m_x, otherPoint1.m_y, otherPoint2.m_x, otherPoint2.m_y));
	}

	float Water::Encode(const float &x, const float &z, const float &x2, const float &z2) const
	{
		float p3 = (x + 1.0f) * 27.0f;
		float p2 = (z + 1.0f) * 9.0f;
		float p1 = (x2 + 1.0f) * 3.0f;
		float p0 = (z2 + 1.0f) * 1.0f;
		return p0 + p1 + p2 + p3;
	}

	float Water::GetHeight(const float &x, const float &z) const
	{
		float waveTime = Engine::Get()->GetTime() / WAVE_SPEED;

		const double val1 = 0.1;
		const double val2 = 0.3;
		double radiansX = 2.0 * PI * ((Maths::Mod(x + z * x * val1, static_cast<double>(WAVE_LENGTH)) / static_cast<double>(WAVE_LENGTH)) + waveTime);
		double radiansZ = 2.0 * PI * ((Maths::Mod(val2 * (z * x + x * z), static_cast<double>(WAVE_LENGTH)) / static_cast<double>(WAVE_LENGTH)) + waveTime * 2.0);
		double result = 0.5 * AMPLITUDE * (cos(radiansZ + sin(x)) + sin(radiansX - cos(z)));
		return static_cast<float>(m_position->m_y + result);
	}

	void Water::SetPosition(const Vector3 &position)
	{
		m_position->Set(position);
		m_moved = true;
	}

	void Water::SetRotation(const Vector3 &rotation)
	{
		m_rotation->Set(rotation);
		m_moved = true;
	}
}
