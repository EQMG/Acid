#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Vector2.hpp"
#include "../maths/Vector4.hpp"

namespace Flounder
{
	class UbosGuis
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
