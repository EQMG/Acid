#pragma once

#include "../maths/Matrix4.hpp"
#include "../maths/Colour.hpp"
#include "../renderer/pipelines/Descriptor.hpp"
#include "../renderer/pipelines/InputState.hpp"

namespace Flounder
{
	class SkyboxesShader
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

		static DescriptorType typeUboScene;
		static DescriptorType typeUboObject;
	//	static DescriptorType typeSamplerCubemap;
		static Descriptor descriptor;

		static InputState inputState;
	};
}
