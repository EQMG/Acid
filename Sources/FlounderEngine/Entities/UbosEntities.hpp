#pragma once

#include "../Maths/Matrix4.hpp"
#include "../Maths/Vector2.hpp"
#include "../Maths/Vector4.hpp"

namespace Flounder
{
	class UbosEntities
	{
	public:
		struct UboScene
		{
			Matrix4 projection;
			Matrix4 view;
			Vector4 clip;
		};

		struct UboObject
		{
			Matrix4 transform;

			float swaying;

			float metallic;
			float roughness;

			Vector2 swayOffset;
		};

		//	static_assert(sizeof(UboObject) % 16 == 0);
	};
}
