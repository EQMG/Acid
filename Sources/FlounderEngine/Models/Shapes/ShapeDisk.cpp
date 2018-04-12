#include "ShapeDisk.hpp"

#include "../../Maths/Maths.hpp"

namespace Flounder
{
	ShapeDisk::ShapeDisk(const float &innerRadius, const float &outerRadius, const unsigned int &slices, const unsigned int &loops) :
		Model()
	{
		std::vector<IVertex*> vertices = std::vector<IVertex*>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		for (unsigned int i = 0; i < slices; i++)
		{
			float iDivSlices = static_cast<float>(i) / static_cast<float>(slices);
			float alpha = iDivSlices * 2.0f * PI;
			float xDir = cos(alpha), yDir = sin(alpha);

			for (unsigned int j = 0; j < loops + 1; j++)
			{
				const float jDivLoops = static_cast<float>(j) / static_cast<float>(loops);
				const float radius = innerRadius + jDivLoops * (outerRadius - innerRadius);

				VertexModel *vertex = new VertexModel();
				vertex->m_normal.m_x = 0.0f;
				vertex->m_normal.m_y = 1.0f;
				vertex->m_normal.m_z = 0.0f;
				vertex->m_uv.m_x = 1.0f - iDivSlices;
				vertex->m_uv.m_y = 1.0f - jDivLoops;
				vertex->m_position.m_x = radius * xDir;
				vertex->m_position.m_y = 0.0f;
				vertex->m_position.m_z = radius * yDir;

				vertices.push_back(vertex);
			}
		}

		for (unsigned int i = 0; i < slices; i++)
		{
			for (unsigned int j = 0; j < loops; j++)
			{
				uint32_t first = i * (loops + 1) + j;
				uint32_t second = (first + loops + 1) % (slices * (loops + 1));

				indices.push_back(first);
				indices.push_back(second + 1);
				indices.push_back(second);

				indices.push_back(first);
				indices.push_back(first + 1);
				indices.push_back(second + 1);
			}
		}

		std::reverse(indices.begin(), indices.end());
		Model::Set(vertices, indices, ToFilename(innerRadius, outerRadius, slices, loops));
	}

	ShapeDisk::~ShapeDisk()
	{
	}

	std::string ShapeDisk::ToFilename(const float &innerRadius, const float &outerRadius, const unsigned int &slices, const unsigned int &loops)
	{
		return "Disk_" + std::to_string(innerRadius) + "_" + std::to_string(outerRadius) + "_" +
			std::to_string(slices) + "_" + std::to_string(loops);
	}
}
