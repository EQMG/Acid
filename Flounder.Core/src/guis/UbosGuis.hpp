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
			Vector4 scissor;
			Vector4 transform;
			float atlasRows;
			Vector2 atlasOffset;
			Colour colourOffset;
			float alpha;
		};
	};
}
