#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Vector2.hpp"
#include "../maths/Vector4.hpp"

namespace Flounder
{
	class UbosFonts
	{
	public:
		struct UboObject
		{
			Vector4 transform;
			Colour colour;
			float alpha;
			Vector4 borderColour;
			Vector2 borderSizes;
			Vector2 edgeData;
		};
	};
}
