#pragma once

#include <string>

#include "../maths/Colour.hpp"
#include "../textures/texture.hpp"

namespace Flounder
{
	/// <summary>
	/// A struct that represents a MTL material.
	/// </summary>
	struct material
	{
		std::string name;

		float specularCoefficient;
		Colour *ambient;
		Colour *diffuse;
		Colour *specular;

		texture *albedo;
		texture *normals;
	};
}
