#include "MeshWater.hpp"

#include "./Maths/Colour.hpp"
#include "WaterRender.hpp"

namespace Flounder
{
	MeshWater::MeshWater() :
		MeshSimple(WaterRender::SIDE_LENGTH, WaterRender::SQUARE_SIZE, WaterRender::VERTEX_COUNT, WaterRender::TEXTURE_SCALE)
	{
		MeshSimple::GenerateMesh();
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
		return WaterRender::WATER_COLOUR;
	}
}
