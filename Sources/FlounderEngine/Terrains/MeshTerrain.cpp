#include "MeshTerrain.hpp"

#include "../Maths/Maths.hpp"
#include "../Maths/Colour.hpp"
#include "Terrains.hpp"

namespace Flounder
{
	const std::array<Colour, 4> biomeColours = {
		Colour("#ffcc00"), Colour("#009933"), Colour("#33cc33"), Colour("#ffffff")
	};
	const float spread = 0.7f;
	const float halfSpread = spread / 2.0f;
	const float amplitude = 15.0f;
	const float part = 1.0f / (biomeColours.size() - 1);

	MeshTerrain::MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, Vector3 *position) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_position(position)
	{
		MeshSimple::GenerateMesh();
	}

	Vector3 MeshTerrain::GetPosition(const float &x, const float &z)
	{
		Vector3 position = Vector3(x, 0.0f, z);
		position.m_y = Terrains::Get()->GetHeight(position.m_x + m_position->m_x, position.m_z + m_position->m_z);
		return position;
	}

	Vector3 MeshTerrain::GetNormal(const Vector3 &position)
	{
		return Terrains::Get()->GetNormal(position.m_x, position.m_z);
	}

	Vector3 MeshTerrain::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		float value = (position.m_y + amplitude) / (amplitude * 2.0f);
		value = Maths::Clamp((value - halfSpread) * (1.0f / spread), 0.0f, 0.9999f);
		int firstBiome = static_cast<int>(floor(value / part));
		float blend = (value - (firstBiome * part)) / part;
		Colour colour = Colour();
		Colour::Interpolate(biomeColours[firstBiome], biomeColours[firstBiome + 1], blend, &colour);
		return colour;
	}
}
