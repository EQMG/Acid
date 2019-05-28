#pragma once

#include "Maths/Matrix4.hpp"
#include "Maths/Vector3.hpp"
#include "Helpers/NonCopyable.hpp"
#include "Animations/Skin/VertexWeights.hpp"
#include "VertexAnimated.hpp"

namespace acid
{
class GeometryLoader :
	public NonCopyable
{
public:
	GeometryLoader(const Metadata *libraryGeometries, std::vector<VertexWeights> vertexWeights, const Matrix4 &correction);

	const std::vector<VertexAnimated> &GetVertices() const { return m_vertices; }

	const std::vector<uint32_t> &GetIndices() const { return m_indices; }

private:
	std::vector<Vector3f> GetPositions();

	std::vector<Vector2f> GetUvs();

	std::vector<Vector3f> GetNormals();

	const Metadata *m_meshData;

	std::vector<VertexWeights> m_vertexWeights;
	Matrix4 m_correction;

	std::vector<VertexAnimated> m_vertices;
	std::vector<uint32_t> m_indices;
};
}
