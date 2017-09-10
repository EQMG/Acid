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
		// TODO: https://github.com/Equilibrium-Games/Flounder-Java-Engine/blob/ab989a6ded24a9feeb45b3adf6a8afdec9a62e5b/src/flounder/materials/Material.java

		std::string name;

		float specularCoefficient;
		Colour *ambient;
		Colour *diffuse;
		Colour *specular;

		Texture *albedo;
		Texture *normals;
	};
}
