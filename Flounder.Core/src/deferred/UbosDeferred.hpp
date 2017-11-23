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

		/*struct Shadow
		{
			Matrix4 shadowSpaceMatrix;
			float shadowDistance;
			float shadowTransition;
			float shadowBias;
			float shadowDarkness;
			int shadowMapSize;
			int shadowPCF;
		};*/

		struct Light
		{
			Colour colour;
			Vector3 position;
		//	Attenuation attenuation;
			Vector2 enabled;
		};

		/*struct Fog
		{
			Colour colour;
			float density;
			float gradient;
		};*/

		struct UboScene
		{
		//	Shadow shadow;
			Light lights[NUMBER_LIGHTS];
		//	Fog fog;

		//	Matrix4 projection;
		//	Matrix4 view;
		};
	};
}
