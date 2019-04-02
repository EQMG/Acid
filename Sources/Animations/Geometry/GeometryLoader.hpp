#pragma once

#include "Maths/Vector3.hpp"
#include "Helpers/NonCopyable.hpp"
#include "Animations/Skin/VertexWeights.hpp"
#include "VertexAnimated.hpp"

namespace acid
{
class ACID_EXPORT GeometryLoader :
	public NonCopyable
{
public:
	GeometryLoader(const Metadata *libraryGeometries, std::vector<VertexWeights> vertexWeights);

	const std::vector<VertexAnimated> &GetVertices() const { return m_vertices; }

	const std::vector<uint32_t> &GetIndices() const { return m_indices; }

private:
	std::vector<Vector3> GetPositions();

	std::vector<Vector2> GetUvs();

	std::vector<Vector3> GetNormals();

	const Metadata *m_meshData;

	std::vector<VertexWeights> m_vertexWeights;

	std::vector<VertexAnimated> m_vertices;
	std::vector<uint32_t> m_indices;
};
}
