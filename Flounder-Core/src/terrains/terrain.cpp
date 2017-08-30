#include "terrain.hpp"

namespace flounder
{
	const float terrain::SQUARE_SIZE = 2.598f;
	const int terrain::VERTEX_COUNT = 176;
	const float terrain::SIDE_LENGTH = 0.5f * SQUARE_SIZE * static_cast<float>(VERTEX_COUNT - 1);

	terrain::terrain(const vector3 &position, const vector3 &rotation, const int &seed) :
		m_model(nullptr),
		m_position(new vector3(position)),
		m_rotation(new vector3(rotation)),
		m_moved(true),
		m_modelMatrix(new matrix4x4()),
		m_aabb(new aabb())
	{
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
		std::vector<float> *vertices = new std::vector<float>();
		std::vector<float> *normals = new std::vector<float>();
		std::vector<float> *colours = new std::vector<float>();
		std::vector<int> *indices = new std::vector<int>();

		colour tint = colour(maths::randomInRange(0.0f, 1.0f), maths::randomInRange(0.0f, 1.0f), 0.0f); // colour(0.0f, 0.8314f, 0.0f);

		for (int col = 0; col < VERTEX_COUNT; col++)
		{
			for (int row = 0; row < VERTEX_COUNT; row++)
			{
				vector3 vertex = vector3((row * SQUARE_SIZE) - (SIDE_LENGTH / 2.0f), 0.0f, (col * SQUARE_SIZE) - (SIDE_LENGTH / 2.0f));
				vertex.m_y = getHeight(vertex.m_x + m_position->m_x - (SIDE_LENGTH / 2.0f), vertex.m_z + m_position->m_z - (SIDE_LENGTH / 2.0f));

				vector3 normal = calculateNormal(vertex.m_x + m_position->m_x - (SIDE_LENGTH / 2.0f), vertex.m_z + m_position->m_z - (SIDE_LENGTH / 2.0f));
				// colour tint = getBiomeColour(vertex.m_x + m_position->m_x, vertex.m_z + m_position->m_z);

				vertices->push_back(vertex.m_x);
				vertices->push_back(vertex.m_y);
				vertices->push_back(vertex.m_z);

				normals->push_back(normal.m_x);
				normals->push_back(normal.m_y);
				normals->push_back(normal.m_z);

				colours->push_back(tint.m_r);
				colours->push_back(tint.m_g);
				colours->push_back(tint.m_b);
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
					storeQuad1(indices, topLeft, topRight, bottomLeft, bottomRight, mixed);
				}
				else
				{
					storeQuad2(indices, topLeft, topRight, bottomLeft, bottomRight, mixed);
				}
			}
		}

		m_model = new model(indices, vertices, nullptr, normals, colours);

		m_aabb->m_maxExtents->m_x = SIDE_LENGTH;
		m_aabb->m_maxExtents->m_z = SIDE_LENGTH;
		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_aabb->update(*m_position, *m_rotation, 1.0f, m_aabb);

		//	delete vertices;
		//	delete normals;
		//	delete colours;
		//	delete indices;
	}

	void terrain::storeQuad1(std::vector<int> *indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed)
	{
		indices->push_back(topLeft);
		indices->push_back(bottomLeft);
		indices->push_back(mixed ? topRight : bottomRight);
		indices->push_back(bottomRight);
		indices->push_back(topRight);
		indices->push_back(mixed ? bottomLeft : topLeft);
	}

	void terrain::storeQuad2(std::vector<int> *indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed)
	{
		indices->push_back(topRight);
		indices->push_back(topLeft);
		indices->push_back(mixed ? bottomRight : bottomLeft);
		indices->push_back(bottomLeft);
		indices->push_back(bottomRight);
		indices->push_back(mixed ? topLeft : topRight);
	}

	vector3 terrain::calculateNormal(const float &x, const float &z)
	{
		float heightL = getHeight(x - SQUARE_SIZE, z);
		float heightR = getHeight(x + SQUARE_SIZE, z);
		float heightD = getHeight(x, z - SQUARE_SIZE);
		float heightU = getHeight(x, z + SQUARE_SIZE);

		vector3 normal = vector3(heightL - heightR, 2.0f, heightD - heightU);
		normal.normalize();
		return normal;
	}

	colour terrain::getBiomeColour(const float &x, const float &z)
	{
		float height = getHeight(x, z);
		float moisture = getFactorMoisture(x, height, z);

		if (height <= 0.125f)
		{
			// Ocean.
			return colour(0.0824f, 0.3960f, 0.7530f);
		}
		else if (height <= 0.25f)
		{
			if (moisture <= 0.16f)
			{
				// Subtropical Desert.
				return colour(0.914f, 0.8275f, 0.7804f);
			}
			else if (moisture <= 0.33f)
			{
				// Grassland.
				return colour(0.7686f, 0.8314f, 0.6666f);
			}
			else if (moisture <= 0.66f)
			{
				// Tropical Seasonal Forest.
				return colour(0.6627f, 0.8000f, 0.6431f);
			}
			else if (moisture <= 1.0f)
			{
				// Tropical Rain Forest.
				return colour(0.6431f, 0.7686f, 0.6588f);
			}
		}
		else if (height <= 0.5f)
		{
			if (moisture <= 0.16f)
			{
				// Temperate Desert.
				return colour(0.8941f, 0.9098f, 0.7922f);
			}
			else if (moisture <= 0.5f)
			{
				// Grassland.
				return colour(0.7686f, 0.8314f, 0.6666f);
			}
			else if (moisture <= 0.83f)
			{
				// Temperate Deciduous Forest.
				return colour(0.7059f, 0.7882f, 0.6627f);
			}
			else if (moisture <= 1.0f)
			{
				// Temperate Rain Forest.
				return colour(0.6431f, 0.7686f, 0.6588f);
			}
		}
		else if (height <= 0.75f)
		{
			if (moisture <= 0.33f)
			{
				// Temperate Desert.
				return colour(0.8941f, 0.9098f, 0.7922f);
			}
			else if (moisture <= 0.66f)
			{
				// Shrubland.
				return colour(0.7686f, 0.8000f, 0.7333f);
			}
			else if (moisture <= 1.0f)
			{
				// Taiga.
				return colour(0.8000f, 0.8314f, 0.7333f);
			}
		}
		else if (height <= 1.0f)
		{
			if (moisture <= 0.16f)
			{
				// Scorched.
				return colour(0.6000f, 0.6000f, 0.6000f);
			}
			else if (moisture <= 0.33f)
			{
				// Bare.
				return colour(0.7333f, 0.7333f, 0.7333f);
			}
			else if (moisture <= 0.5f)
			{
				// Tundra.
				return colour(0.8666f, 0.8666f, 0.7333f);
			}
			else if (moisture <= 1.0f)
			{
				// Snow.
				return colour(1.0000f, 1.0000f, 1.0000f);
			}
		}

		return colour(0.0824f, 0.3960f, 0.7530f);
	}

	float terrain::getFactorIsland(const float &x, const float &z)
	{
		const float worldSize = SIDE_LENGTH * 6;
		const float worldIslandInside = 0.8f;
		const float worldIslandOutside = 1.0f;
		const float worldIslandParameter = 0.4f;

		float circular = sqrt(pow(x, 2) + pow(z, 2)); // The current radius (circular map).
		float rectangular = __max(fabs(x), fabs(z)); // The current radius (rectangular map).
		float reading = ((1.0f - worldIslandParameter) * circular) + (worldIslandParameter * rectangular);

		float radius1 = worldIslandInside * (worldSize / 2.0f); // The inside radius to the blur.
		float radius2 = worldIslandOutside * (worldSize / 2.0f); // The outside radius to the blur.

		if (x == 0.0f && z == 0.0f) // The special case where the reading is undefined.
		{
			return 1.0f;
		}
		else if (reading > radius2) // If outside the upper bound there is no factor!
		{
			return 0.0f;
		}
		else if (reading >= radius1) // Something between upper and lower, uses cos interpolation.
		{
			float blend = maths::clamp((reading - radius1) / (radius2 - radius1), 0.0f, 1.0f);
			return maths::clamp(maths::cosInterpolate(1.0f, 0.0f, blend), 0.0f, 1.0f);
		}
		else // Fully inside of the lower radius, so full factor.
		{
			return 1.0f;
		}
	}

	float terrain::getFactorMoisture(const float &x, const float &y, const float &z)
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
			moisture += worlds::get()->getNoise()->getNoise(x / 1.5f, z / 1.5f); // TODO
		}

		moisture = maths::clamp(moisture, 0.0f, 1.0f);
		return moisture;
	}

	float terrain::getHeight(const float &x, const float &z)
	{
		const float worldNoiseSpread = 1.0f;
		const float worldNoiseFrequency = 40.0f;
		const float worldNoiseHeight = 40.0f;
		const float dayNightCycle = 600.0f;
		const float dayNightRatio = 0.7f;

		float height = worlds::get()->getNoise()->getNoise(x / worldNoiseSpread, z / worldNoiseSpread, worldNoiseFrequency);
		height *= getFactorIsland(x, z);
		height *= worldNoiseHeight;
		return height;
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
