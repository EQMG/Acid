#include "ShapeCylinder.hpp"

#include "Maths/Maths.hpp"

namespace fl
{
	ShapeCylinder::ShapeCylinder(const float &radiusBase, const float &radiusTop, const float &height, const unsigned int &slices, const unsigned int &stacks, const float &y0) :
		Model()
	{
		auto vertices = std::vector<IVertex *>();
		auto indices = std::vector<uint32_t>();

		for (unsigned int i = 0; i < slices + 1; i++)
		{
			float iDivSlices = static_cast<float>(i) / static_cast<float>(slices);
			float alpha = (i == 0 || i == slices) ? 0.0f : iDivSlices * 2.0f * PI;
			float xDir = std::cos(alpha);
			float zDir = std::sin(alpha);

			for (unsigned int j = 0; j < stacks + 1; j++)
			{
				float jDivStacks = static_cast<float>(j) / static_cast<float>(stacks);
				float radius = radiusBase * (1.0f - jDivStacks) + radiusTop * jDivStacks;

				VertexModel *vertex = new VertexModel();
				vertex->m_position.m_x = xDir * radius;
				vertex->m_position.m_y = y0 + jDivStacks * height;
				vertex->m_position.m_z = zDir * radius;
				vertex->m_uv.m_x = 1.0f - iDivSlices;
				vertex->m_uv.m_y = 1.0f - jDivStacks;
				vertex->m_normal.m_x = xDir;
				vertex->m_normal.m_y = 0.0f;
				vertex->m_normal.m_z = zDir;

				vertices.emplace_back(vertex);
			}
		}

		for (unsigned int i = 0; i < slices; i++)
		{
			for (unsigned int j = 0; j < stacks; j++)
			{
				uint32_t first = j + ((stacks + 1) * i);
				uint32_t second = j + ((stacks + 1) * (i + 1));

				indices.emplace_back(first);
				indices.emplace_back(second);
				indices.emplace_back(second + 1);

				indices.emplace_back(first);
				indices.emplace_back(second + 1);
				indices.emplace_back(first + 1);
			}
		}

		std::reverse(indices.begin(), indices.end());
		Model::Set(vertices, indices, ToFilename(radiusBase, radiusTop, height, slices, stacks, y0));
	}

	ShapeCylinder::~ShapeCylinder()
	{
	}

	std::string ShapeCylinder::ToFilename(const float &radiusBase, const float &radiusTop, const float &height, const unsigned int &slices, const unsigned int &stacks, const float &y0)
	{
		return "Cylinder_" + std::to_string(radiusBase) + "_" + std::to_string(radiusTop) + "_" + std::to_string(height) + "_" +
			std::to_string(slices) + "_" + std::to_string(stacks) + "_" + std::to_string(y0);
	}
}
