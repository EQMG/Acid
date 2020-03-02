#pragma once

#include "Maths/Matrix4.hpp"
#include "Maths/Vector3.hpp"
#include "Utils/NonCopyable.hpp"
#include "Animations/Skin/VertexWeights.hpp"
#include "VertexAnimated.hpp"

namespace acid {
class ACID_EXPORT GeometryLoader : NonCopyable {
public:
	GeometryLoader(NodeConstView &&libraryGeometries, std::vector<VertexWeights> vertexWeights, const Matrix4 &correction);

	const std::vector<VertexAnimated> &GetVertices() const { return vertices; }
	const std::vector<uint32_t> &GetIndices() const { return indices; }

private:
	std::vector<Vector3f> GetPositions() const;
	std::vector<Vector2f> GetUvs() const;
	std::vector<Vector3f> GetNormals() const;

	NodeConstView meshData;

	std::vector<VertexWeights> vertexWeights;
	Matrix4 correction;

	std::vector<VertexAnimated> vertices;
	std::vector<uint32_t> indices;
};
}
