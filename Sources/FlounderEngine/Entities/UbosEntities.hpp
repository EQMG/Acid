#pragma once

#include "../Maths/Colour.hpp"
#include "../Maths/Matrix4.hpp"
#include "../Maths/Vector2.hpp"
#include "../Maths/Vector4.hpp"

namespace Flounder
{
	class F_EXPORT UbosEntities
	{
	public:
#define MAX_JOINTS 50
#define MAX_WEIGHTS 3

		struct UboScene
		{
			Matrix4 projection;
			Matrix4 view;
		};

		struct UboObject
		{
		//	Matrix4 jointTransforms[MAX_JOINTS];

			Matrix4 transform;

			Vector4 baseColor;
			float metallic;
			float roughness;
			float ignoreFog;
			float ignoreLighting;
		};
	};
}
