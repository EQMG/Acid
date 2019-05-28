#pragma once

#include "Models/Model.hpp"
#include "Models/VertexDefault.hpp"

namespace acid
{
class MeshPattern :
	public Model
{
public:
	MeshPattern(const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &uvScale);

protected:
	void GenerateMesh();

	virtual VertexDefault GetVertex(const uint32_t &col, const uint32_t &row);

	float m_sideLength;
	float m_squareSize;
	uint32_t m_vertexCount;
	float m_uvScale;
};
}
