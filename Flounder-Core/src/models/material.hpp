#pragma once

#include <string>

#include "../maths/colour.hpp"
#include "../textures/texture.hpp"

namespace flounder
{
	/// <summary>
	/// A struct that represents a MTL material.
	/// </summary>
	struct material
	{
		std::string name;

		float specularCoefficient;
		colour *ambient;
		colour *diffuse;
		colour *specular;

		texture *albedo;
		texture *normals;
	};
}
