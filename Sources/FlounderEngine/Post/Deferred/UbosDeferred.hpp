#pragma once

#include "../../Maths/Colour.hpp"
#include "../../Maths/Vector3.hpp"
#include "../../Maths/Matrix4.hpp"

namespace Flounder
{
	class F_EXPORT UbosDeferred
	{
	public:
#define MAX_LIGHTS 64

		struct Light
		{
			Colour colour;
			Vector3 position;
			float radius;
		};

		struct UboScene
		{
			Light lights[MAX_LIGHTS];

			Matrix4 projection;
			Matrix4 view;
			Matrix4 shadowSpace;

			Colour fogColour;
			Vector3 cameraPosition;
			float fogDensity;
			float fogGradient;

			float shadowDistance;
			float shadowTransition;
			float shadowBias;
			float shadowDarkness;
			int shadowPCF;

			int lightsCount;
		};
	};
}
