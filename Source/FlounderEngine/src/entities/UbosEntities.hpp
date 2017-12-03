#pragma once

#include "../maths/Matrix4.hpp"
#include "../maths/Vector2.hpp"
#include "../maths/Vector4.hpp"

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
