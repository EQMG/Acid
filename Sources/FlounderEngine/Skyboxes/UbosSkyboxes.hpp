#pragma once

#include "../Maths/Colour.hpp"
#include "../Maths/Matrix4.hpp"
#include "../Maths/Vector4.hpp"

namespace Flounder
{
	class F_EXPORT UbosSkyboxes
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
			Colour fogColour;
			Vector2 fogLimits;
			float blendFactor;
		};
	};
}
