#include "MeshWater.hpp"

#include "./Maths/Colour.hpp"
#include "Water.hpp"

namespace Flounder
{
	MeshWater::MeshWater(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale)
	{
	}

	Vector3 MeshWater::GetPosition(const float &x, const float &z)
	{
		return Vector3(x, 0.0f, z);
	}

	Vector3 MeshWater::GetNormal(const Vector3 &position)
	{
		return Vector3(0.0f, 1.0f, 0.0f);
	}

	Vector3 MeshWater::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		return Water::WATER_COLOUR;
	}
}
