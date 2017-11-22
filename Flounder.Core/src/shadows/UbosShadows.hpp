#pragma once

#include "../maths/Matrix4.hpp"

namespace Flounder
{
	class UbosShadows
	{
	public:
		struct UboObject
		{
			Matrix4 mvp;
		};
	};
}
