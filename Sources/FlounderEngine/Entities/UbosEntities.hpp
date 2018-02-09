#pragma once

#include "../Maths/Colour.hpp"
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

			Vector4 diffuse;

			Vector4 samples;

			Vector3 surface;
			Vector2 swaying;
		};

	//	static_assert(sizeof(UboObject) % 16 == 0);
	};
}
