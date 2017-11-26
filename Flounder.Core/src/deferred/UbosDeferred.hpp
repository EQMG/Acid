#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Vector3.hpp"
#include "../maths/Matrix4.hpp"

namespace Flounder
{
	class UbosDeferred
	{
	public:
#define MAX_LIGHTS 63

		struct Light
		{
			Colour colour;
			Vector3 position;
			float radius;
		};

		struct UboScene
		{
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
			int shadowMapSize;
			int shadowPCF;

			int lightsCount;
		};

		struct UboLights
		{
			Light lights[MAX_LIGHTS];
		};

	//	static_assert(sizeof(Light) % 16 == 0);
	//	static_assert(sizeof(UboScene) % 16 == 0);
	//	static_assert(sizeof(UboLights) % 16 == 0);
	};
}
