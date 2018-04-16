#pragma once

#include "Maths/Matrix4.hpp"

namespace Flounder
{
	class F_EXPORT UbosShadows
	{
	public:
		struct UboScene
		{
			Matrix4 projectionView;
			Vector3 cameraPosition;
		};

		struct UboObject
		{
			Matrix4 transform;
		};
	};
}
