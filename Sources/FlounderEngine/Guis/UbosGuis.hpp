#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"

namespace Flounder
{
	class F_EXPORT UbosGuis
	{
	public:
		struct UboObject
		{
			Vector4 transform;
			Colour colourOffset;
			Vector2 atlasOffset;
			float atlasRows;
			float alpha;
		};
	};
}
