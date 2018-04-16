#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"

namespace Flounder
{
	class UbosFonts
	{
	public:
		struct F_EXPORT UboObject
		{
			Vector4 transform;
			Colour colour;
			Colour borderColour;
			Vector2 borderSizes;
			Vector2 edgeData;
			float alpha;
		};
	};
}
