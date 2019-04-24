#pragma once

#include <Models/Shapes/MeshSimple.hpp>
#include <Maths/Maths.hpp>

using namespace acid;

namespace test
{
class MeshTerrain :
	public MeshSimple
{
public:
	MeshTerrain(const std::vector<float> &heightmap, const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &textureScale);

	VertexDefault GetVertex(const uint32_t &col, const uint32_t &row) override;

private:
	Vector3f GetPosition(const float &x, const float &z);

	Vector3f GetNormal(const float &x, const float &z);

	Colour GetColour(const Vector3f &normal);

	std::vector<float> m_heightmap;
};
}
