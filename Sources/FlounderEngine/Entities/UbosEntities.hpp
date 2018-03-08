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
		struct UboScene
		{
			Matrix4 projection;
			Matrix4 view;
		};

		struct UboObject
		{
			Matrix4 transform;

			Vector4 baseColor;

			Vector4 samples;

			Vector4 surface;
		};
	};
}
