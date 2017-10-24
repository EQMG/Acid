#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Vector2.hpp"
#include "../maths/Vector4.hpp"

namespace Flounder
{
	class UbosFonts
	{
	public:
		struct UboScene
		{
			float aspectRatio;
		};

		struct UboObject
		{
			Vector4 scissor;
			Vector2 size;
			Vector4 transform;
			float rotation;
			Colour colour;
			Vector4 borderColour;
			Vector2 borderSizes;
			Vector2 edgeData;
		};
	};
}
