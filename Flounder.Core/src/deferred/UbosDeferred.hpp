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
		struct Light
		{
			Colour colour;
			Vector3 position;
			Attenuation attenuation;
			float enabled;
		};

		struct Shadow
		{
			Matrix4 shadowSpaceMatrix;
			float shadowDistance;
			float shadowTransition;
			float shadowBias;
			float shadowDarkness;
			int shadowMapSize;
			int shadowPCF;
		};

		struct Fog
		{
			Colour colour;
			float density;
			float gradient;
		};

#define NUMBER_LIGHTS 32

		struct UboScene
		{
			Light lights[NUMBER_LIGHTS];
			Shadow shadow;
			Fog fog;

			Matrix4 projection;
			Matrix4 view;
		};
	};
}
