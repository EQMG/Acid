#pragma once

#include "../Maths/Matrix4.hpp"
#include "../Maths/Vector4.hpp"

namespace Flounder
{
	class F_EXPORT UbosTerrains
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

			float worldCurvature;
		};
	};
}
