#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Vector2.hpp"
#include "../maths/Vector4.hpp"

namespace Flounder
{
	class UbosGuis
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
			float alpha;
			float flipTexture;
			float atlasRows;
			Vector2 atlasOffset;
			Colour colourOffset;
		};
	};
}
