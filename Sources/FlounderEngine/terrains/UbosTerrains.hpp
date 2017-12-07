#pragma once

#include "../Maths/Matrix4.hpp"
#include "../Maths/Vector4.hpp"

namespace Flounder
{
	class UbosTerrains
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
		};
	};
}
