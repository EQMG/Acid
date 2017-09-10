#pragma once

#include <string>

#include "../maths/Colour.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	/// <summary>
	/// A struct that represents a MTL material.
	/// </summary>
	struct Material
	{
		std::string name;

		float specularCoefficient;
		Colour *ambient;
		Colour *diffuse;
		Colour *specular;

		Texture *albedo;
		Texture *normals;
	};
}
