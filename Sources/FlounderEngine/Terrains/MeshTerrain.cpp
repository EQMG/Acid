#include "MeshTerrain.hpp"

#include "../Maths/Maths.hpp"
#include "../Maths/Colour.hpp"
#include "Terrains.hpp"

namespace Flounder
{
	const std::array<Colour, 4> BIOME_COLOURS = {
		Colour("#ffcc00"), Colour("#E17B35"), Colour("#C35D35"), Colour("#683130")
	};
	const float SPREAD = 0.76f;
	const float HALF_SPREAD = SPREAD / 2.0f;
	const float AMPLITUDE = 20.0f;
	const float PART = 1.0f / (BIOME_COLOURS.size() - 1);

	MeshTerrain::MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, Vector3 *position) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_position(position)
	{
		MeshSimple::GenerateMesh();
	}

	Vector3 MeshTerrain::GetPosition(const float &x, const float &z)
	{
		// Map from the cube [-1,1] (x, y, z) to the unit sphere.
		// http://mathproofs.blogspot.com/2005/07/mapping-cube-to-sphere.html
		Vector3 position = Vector3(x, 0.0f, z);
		position.m_y = Terrains::Get()->GetHeight(position.m_x + m_position->m_x, position.m_z + m_position->m_z);
		/*float x2 = position.m_x * position.m_x;
		float y2 = position.m_y * position.m_y;
		float z2 = position.m_z * position.m_z;
		position.Set(position.m_x * std::sqrt(1.0f - (y2 + z2) * 0.5f + y2 * z2 * 0.33333333333333333333f),
			position.m_y * std::sqrt(1.0f - (z2 + x2) * 0.5f + z2 * x2 * 0.33333333333333333333f),
			position.m_z * std::sqrt(1.0f - (x2 + y2) * 0.5f + x2 * y2 * 0.33333333333333333333f));*/
		return position;
	}

	Vector3 MeshTerrain::GetNormal(const Vector3 &position)
	{
		return Terrains::Get()->GetNormal(position.m_x + m_position->m_x, position.m_z + m_position->m_z);
	}

	Vector3 MeshTerrain::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		float value = (position.m_y + AMPLITUDE) / (AMPLITUDE * 2.0f);
		value = Maths::Clamp((value - HALF_SPREAD) * (1.0f / SPREAD), 0.0f, 0.9999f);
		int firstBiome = static_cast<int>(std::floor(value / PART));
		float blend = (value - (firstBiome * PART)) / PART;
		Colour colour = Colour();
		Colour::Interpolate(BIOME_COLOURS.at(firstBiome), BIOME_COLOURS.at(firstBiome + 1), blend, &colour);
		return colour;
	}
}
