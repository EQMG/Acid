#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Matrix4.hpp"
#include "../maths/Vector4.hpp"

namespace Flounder
{
	class UbosSkyboxes
	{
	public:
		struct UboScene
		{
			Matrix4 projection;
			Matrix4 view;
			Vector4 clip;
		};

		struct UboObject
		{
			Matrix4 transform;
			Colour skyColour;
			float blendFactor;
		};
	};
}
