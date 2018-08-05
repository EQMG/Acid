#include "ModelSphere.hpp"

#include "Maths/Maths.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	ModelSphere::ModelSphere(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius) :
		Model()
	{
		std::vector<IVertex *> vertices = std::vector<IVertex *>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		for (uint32_t i = 0; i < longitudeBands + 1; i++)
		{
			float iDivLong = static_cast<float>(i) / static_cast<float>(longitudeBands);
			float theta = (i == 0 || i == longitudeBands) ? 0.0f : iDivLong * 2.0f * PI;

			for (uint32_t j = 0; j < latitudeBands + 1; j++)
			{
				float jDivLat = static_cast<float>(j) / static_cast<float>(latitudeBands);
				float phi = jDivLat * 2.0f * PI;

				VertexModel *vertex = new VertexModel();
				vertex->m_normal.m_x = std::cos(phi) * std::sin(theta);
				vertex->m_normal.m_y = std::cos(theta);
				vertex->m_normal.m_z = std::sin(phi) * std::sin(theta);
				vertex->m_uv.m_x = 1.0f - jDivLat;
				vertex->m_uv.m_y = 1.0f - iDivLong;
				vertex->m_position.m_x = radius * vertex->m_normal.m_x;
				vertex->m_position.m_y = radius * vertex->m_normal.m_y;
				vertex->m_position.m_z = radius * vertex->m_normal.m_z;

				vertices.emplace_back(vertex);
			}
		}

		for (uint32_t i = 0; i < longitudeBands; i++)
		{
			for (uint32_t j = 0; j < latitudeBands; j++)
			{
				uint32_t first = j + ((latitudeBands + 1) * i);
				uint32_t second = j + ((latitudeBands + 1) * (i + 1));

				indices.emplace_back(first);
				indices.emplace_back(second + 1);
				indices.emplace_back(second);

				indices.emplace_back(first);
				indices.emplace_back(first + 1);
				indices.emplace_back(second + 1);
			}
		}

		std::reverse(indices.begin(), indices.end());
		Model::Set(vertices, indices, ToFilename(latitudeBands, longitudeBands, radius));
	}

	ModelSphere::~ModelSphere()
	{
	}

	std::string ModelSphere::ToFilename(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius)
	{
		return "Sphere_" + std::to_string(latitudeBands) + "_" + std::to_string(longitudeBands) + "_" +
			std::to_string(radius);
	}
}
