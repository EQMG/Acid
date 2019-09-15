#pragma once

#include "Models/Model.hpp"
#include "Models/VertexDefault.hpp"

namespace acid {
class ACID_EXPORT
MeshSimple
:
public
Model
{
public:
	MeshSimple(float sideLength, float squareSize, uint32_t vertexCount, float uvScale);

protected:
	void GenerateMesh();

	virtual VertexDefault GetVertex(uint32_t col, uint32_t row);

	float m_sideLength;
	float m_squareSize;
	uint32_t m_vertexCount;
	float m_uvScale;
};
}
