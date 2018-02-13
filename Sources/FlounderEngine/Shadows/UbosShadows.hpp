#pragma once

#include "../Maths/Matrix4.hpp"

namespace Flounder
{
	class F_EXPORT UbosShadows
	{
	public:
		struct UboObject
		{
			Matrix4 mvp;
		};
	};
}
