#pragma once

#include "../Maths/Matrix4.hpp"

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
