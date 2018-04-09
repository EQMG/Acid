#include "ShapeSphere.hpp"

#include "../../Maths/Maths.hpp"

namespace Flounder
{
	ShapeSphere::ShapeSphere(const int &latitudeBands, const int &longitudeBands, const float &radius) :
		Model()
	{
		std::vector<IVertex*> vertices = std::vector<IVertex*>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		for (int latNumber = 0; latNumber <= latitudeBands; latNumber++)
		{
			float theta = latNumber * PI / latitudeBands;
			float sinTheta = std::sin(theta);
			float cosTheta = std::cos(theta);

			for (int longNumber = 0; longNumber <= longitudeBands; longNumber++)
			{
				float phi = longNumber * 2.0f * PI / longitudeBands;
				float sinPhi = std::sin(phi);
				float cosPhi = std::cos(phi);

				VertexModel *vertex = new VertexModel();
				vertex->m_normal.m_x = cosPhi * sinTheta;
				vertex->m_normal.m_y = cosTheta;
				vertex->m_normal.m_z = sinPhi * sinTheta;
				vertex->m_uv.m_x = 1.0f - (longNumber / longitudeBands);
				vertex->m_uv.m_y = 1.0f - (latNumber / latitudeBands);
				vertex->m_position.m_x = radius * vertex->m_normal.m_x;
				vertex->m_position.m_y = radius * vertex->m_normal.m_y;
				vertex->m_position.m_z = radius * vertex->m_normal.m_z;

				vertices.push_back(vertex);
			}
		}

		for (int latNumber = 0; latNumber < latitudeBands; latNumber++)
		{
			for (int longNumber = 0; longNumber < longitudeBands; longNumber++)
			{
				uint32_t first = (latNumber * (longitudeBands + 1)) + longNumber;
				uint32_t second = first + longitudeBands + 1;

				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);

				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}

		std::reverse(indices.begin(), indices.end());
		Model::Set(vertices, indices, ToFilename(latitudeBands, longitudeBands, radius));
	}

	ShapeSphere::~ShapeSphere()
	{
	}

	std::string ShapeSphere::ToFilename(const int &latitudeBands, const int &longitudeBands, const float &radius)
	{
		return "Sphere_" + std::to_string(latitudeBands) + "_" + std::to_string(longitudeBands) + "_" +
			std::to_string(radius);
	}
}
