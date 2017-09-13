#include "Terrain.hpp"

namespace Flounder
{
	const float Terrain::SQUARE_SIZE = 2.598f;
	const int Terrain::VERTEX_COUNT = 176;
	const float Terrain::SIDE_LENGTH = 0.5f * SQUARE_SIZE * static_cast<float>(VERTEX_COUNT - 1);

	Terrain::Terrain(const Vector3 &position, const Vector3 &rotation, const int &seed) :
		m_model(nullptr),
		m_position(new Vector3(position)),
		m_rotation(new Vector3(rotation)),
		m_moved(true),
		m_modelMatrix(new Matrix4()),
		m_aabb(new Aabb())
	{
		GenerateMesh();
	}

	Terrain::~Terrain()
	{
		delete m_model;

		delete m_position;
		delete m_rotation;

		delete m_modelMatrix;
		delete m_aabb;
	}

	void Terrain::Update()
	{
		if (m_moved)
		{
			Matrix4::TransformationMatrix(*m_position, *m_rotation, 1.0f, m_modelMatrix);
			m_moved = false;
		}
	}

	void Terrain::GenerateMesh()
	{
		int count = VERTEX_COUNT * VERTEX_COUNT;
		std::vector<float> vertices = std::vector<float>();
		std::vector<float> textures = std::vector<float>();
		std::vector<float> normals = std::vector<float>();
		std::vector<float> colours = std::vector<float>();
		std::vector<int> indices = std::vector<int>();

		Colour tint = Colour(Maths::RandomInRange(0.0f, 1.0f), Maths::RandomInRange(0.0f, 1.0f), 0.0f, 1.0f); // colour(0.0f, 0.8314f, 0.0f);

		for (int col = 0; col < VERTEX_COUNT; col++)
		{
			for (int row = 0; row < VERTEX_COUNT; row++)
			{
				Vector3 vertex = Vector3((row * SQUARE_SIZE) - (SIDE_LENGTH / 2.0f), 0.0f, (col * SQUARE_SIZE) - (SIDE_LENGTH / 2.0f));
				vertex.m_y = GetHeight(vertex.m_x + m_position->m_x - (SIDE_LENGTH / 2.0f), vertex.m_z + m_position->m_z - (SIDE_LENGTH / 2.0f));

				Vector3 normal = CalculateNormal(vertex.m_x + m_position->m_x - (SIDE_LENGTH / 2.0f), vertex.m_z + m_position->m_z - (SIDE_LENGTH / 2.0f));
				// colour tint = getBiomeColour(vertex.m_x + m_position->m_x, vertex.m_z + m_position->m_z);

				vertices.push_back(vertex.m_x);
				vertices.push_back(vertex.m_y);
				vertices.push_back(vertex.m_z);

				textures.push_back(0.0f);
				textures.push_back(0.0f);

				normals.push_back(normal.m_x);
				normals.push_back(normal.m_y);
				normals.push_back(normal.m_z);

				colours.push_back(tint.m_r);
				colours.push_back(tint.m_g);
				colours.push_back(tint.m_b);
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
				bool mixed = col % 2 == 0;

				if (row % 2 == 0)
				{
					StoreQuad1(indices, topLeft, topRight, bottomLeft, bottomRight, mixed);
				}
				else
				{
					StoreQuad2(indices, topLeft, topRight, bottomLeft, bottomRight, mixed);
				}
			}
		}

		m_model = new Model(indices, vertices, textures, normals, colours);

		m_aabb->m_maxExtents->m_x = SIDE_LENGTH;
		m_aabb->m_maxExtents->m_z = SIDE_LENGTH;
		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_aabb->Update(*m_position, *m_rotation, 1.0f, m_aabb);

		//	delete vertices;
		//	delete normals;
		//	delete colours;
		//	delete indices;
	}

	void Terrain::StoreQuad1(std::vector<int> &indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed)
	{
		indices.push_back(topLeft);
		indices.push_back(bottomLeft);
		indices.push_back(mixed ? topRight : bottomRight);
		indices.push_back(bottomRight);
		indices.push_back(topRight);
		indices.push_back(mixed ? bottomLeft : topLeft);
	}

	void Terrain::StoreQuad2(std::vector<int> &indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed)
	{
		indices.push_back(topRight);
		indices.push_back(topLeft);
		indices.push_back(mixed ? bottomRight : bottomLeft);
		indices.push_back(bottomLeft);
		indices.push_back(bottomRight);
		indices.push_back(mixed ? topLeft : topRight);
	}

	Vector3 Terrain::CalculateNormal(const float &x, const float &z)
	{
		float heightL = GetHeight(x - SQUARE_SIZE, z);
		float heightR = GetHeight(x + SQUARE_SIZE, z);
		float heightD = GetHeight(x, z - SQUARE_SIZE);
		float heightU = GetHeight(x, z + SQUARE_SIZE);

		Vector3 normal = Vector3(heightL - heightR, 2.0f, heightD - heightU);
		normal.Normalize();
		return normal;
	}

	Colour Terrain::GetBiomeColour(const float &x, const float &z)
	{
		float height = GetHeight(x, z);
		float moisture = GetFactorMoisture(x, height, z);

		if (height <= 0.125f)
		{
			// Ocean.
			return Colour(0.0824f, 0.3960f, 0.7530f);
		}
		if (height <= 0.25f)
		{
			if (moisture <= 0.16f)
			{
				// Subtropical Desert.
				return Colour(0.914f, 0.8275f, 0.7804f);
			}
			if (moisture <= 0.33f)
			{
				// Grassland.
				return Colour(0.7686f, 0.8314f, 0.6666f);
			}
			if (moisture <= 0.66f)
			{
				// Tropical Seasonal Forest.
				return Colour(0.6627f, 0.8000f, 0.6431f);
			}
			if (moisture <= 1.0f)
			{
				// Tropical Rain Forest.
				return Colour(0.6431f, 0.7686f, 0.6588f);
			}
		}
		else if (height <= 0.5f)
		{
			if (moisture <= 0.16f)
			{
				// Temperate Desert.
				return Colour(0.8941f, 0.9098f, 0.7922f);
			}
			if (moisture <= 0.5f)
			{
				// Grassland.
				return Colour(0.7686f, 0.8314f, 0.6666f);
			}
			if (moisture <= 0.83f)
			{
				// Temperate Deciduous Forest.
				return Colour(0.7059f, 0.7882f, 0.6627f);
			}
			if (moisture <= 1.0f)
			{
				// Temperate Rain Forest.
				return Colour(0.6431f, 0.7686f, 0.6588f);
			}
		}
		else if (height <= 0.75f)
		{
			if (moisture <= 0.33f)
			{
				// Temperate Desert.
				return Colour(0.8941f, 0.9098f, 0.7922f);
			}
			if (moisture <= 0.66f)
			{
				// Shrubland.
				return Colour(0.7686f, 0.8000f, 0.7333f);
			}
			if (moisture <= 1.0f)
			{
				// Taiga.
				return Colour(0.8000f, 0.8314f, 0.7333f);
			}
		}
		else if (height <= 1.0f)
		{
			if (moisture <= 0.16f)
			{
				// Scorched.
				return Colour(0.6000f, 0.6000f, 0.6000f);
			}
			if (moisture <= 0.33f)
			{
				// Bare.
				return Colour(0.7333f, 0.7333f, 0.7333f);
			}
			if (moisture <= 0.5f)
			{
				// Tundra.
				return Colour(0.8666f, 0.8666f, 0.7333f);
			}
			if (moisture <= 1.0f)
			{
				// Snow.
				return Colour(1.0000f, 1.0000f, 1.0000f);
			}
		}

		return Colour(0.0824f, 0.3960f, 0.7530f);
	}

	float Terrain::GetFactorIsland(const float &x, const float &z)
	{
		const float worldSize = SIDE_LENGTH * 6;
		const float worldIslandInside = 0.8f;
		const float worldIslandOutside = 1.0f;
		const float worldIslandParameter = 0.4f;

		float circular = sqrt(pow(x, 2) + pow(z, 2)); // The current radius (circular map).
		float rectangular = Maths::Max(fabs(x), fabs(z)); // The current radius (rectangular map).
		float reading = ((1.0f - worldIslandParameter) * circular) + (worldIslandParameter * rectangular);

		float radius1 = worldIslandInside * (worldSize / 2.0f); // The inside radius to the blur.
		float radius2 = worldIslandOutside * (worldSize / 2.0f); // The outside radius to the blur.

		if (x == 0.0f && z == 0.0f) // The special case where the reading is undefined.
		{
			return 1.0f;
		}
		if (reading > radius2) // If outside the upper bound there is no factor!
		{
			return 0.0f;
		}
		if (reading >= radius1) // Something between upper and lower, uses cos interpolation.
		{
			float blend = Maths::Clamp((reading - radius1) / (radius2 - radius1), 0.0f, 1.0f);
			return Maths::Clamp(Maths::CosInterpolate(1.0f, 0.0f, blend), 0.0f, 1.0f);
		}
		// Fully inside of the lower radius, so full factor.
		return 1.0f;
	}

	float Terrain::GetFactorMoisture(const float &x, const float &y, const float &z)
	{
		const float worldNoiseHeight = 40.0f;

		// Calculate the moisture as a inverse of height with added noise.
		float moisture = y / worldNoiseHeight;

		// Set to 100% moisture in the ocean/lakes/rivers.
		if ((y / worldNoiseHeight) <= 0.0f)
		{
			moisture = 1.0f;
		}
		else
		{
			moisture += Worlds::Get()->GetNoise()->GetNoise(x / 1.5f, z / 1.5f); // TODO
		}

		moisture = Maths::Clamp(moisture, 0.0f, 1.0f);
		return moisture;
	}

	float Terrain::GetHeight(const float &x, const float &z)
	{
		const float worldNoiseSpread = 1.0f;
		const float worldNoiseFrequency = 40.0f;
		const float worldNoiseHeight = 40.0f;
		const float dayNightCycle = 600.0f;
		const float dayNightRatio = 0.7f;

		float height = Worlds::Get()->GetNoise()->GetNoise(x / worldNoiseSpread, z / worldNoiseSpread, worldNoiseFrequency);
		height *= GetFactorIsland(x, z);
		height *= worldNoiseHeight;
		return height;
	}

	void Terrain::SetPosition(const Vector3 &position)
	{
		m_position->Set(position);
		m_moved = true;
	}

	void Terrain::SetRotation(const Vector3 &rotation)
	{
		m_rotation->Set(rotation);
		m_moved = true;
	}
}
