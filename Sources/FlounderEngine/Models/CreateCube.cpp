#include "CreateCube.hpp"

#include <algorithm>
#include "../Maths/Maths.hpp"

namespace Flounder
{
	Model *CreateCube::Create(const float &width, const float &height, const float &depth)
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		return new Model(vertices, indices);
	}
}