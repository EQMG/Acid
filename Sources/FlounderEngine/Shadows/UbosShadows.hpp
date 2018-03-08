#pragma once

#include "../Maths/Matrix4.hpp"

namespace Flounder
{
	class F_EXPORT UbosShadows
	{
	public:
		struct UboScene
		{
			Matrix4 projectionView;
		};

		struct UboObject
		{
			Matrix4 transform;
		};
	};
}
