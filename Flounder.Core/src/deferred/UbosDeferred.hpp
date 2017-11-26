#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Vector3.hpp"
#include "../maths/Matrix4.hpp"
#include "../lights/Attenuation.hpp"

namespace Flounder
{
	class UbosDeferred
	{
	public:
#define NUMBER_LIGHTS 32

		struct UboScene
		{
			Colour lightColours[NUMBER_LIGHTS];
			Vector3 lightPositions[NUMBER_LIGHTS];
			Attenuation lightAttenuations[NUMBER_LIGHTS];

			Matrix4 projection;
			Matrix4 view;
			Matrix4 shadowSpace;

			Colour fogColour;
			float fogDensity;
			float fogGradient;

			float shadowDistance;
			float shadowTransition;
			float shadowBias;
			float shadowDarkness;
			int shadowMapSize;
			int shadowPCF;
		};
	};
}
