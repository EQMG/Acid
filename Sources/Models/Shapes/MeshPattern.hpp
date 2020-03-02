#pragma once

#include "Models/Model.hpp"
#include "Models/Vertex3d.hpp"

namespace acid {
class ACID_EXPORT MeshPattern : public Model {
public:
	MeshPattern(float sideLength, float squareSize, uint32_t vertexCount, float uvScale);

protected:
	virtual Vertex3d GetVertex(uint32_t col, uint32_t row);

	void GenerateMesh();

	float sideLength;
	float squareSize;
	uint32_t vertexCount;
	float uvScale;
};
}
