#pragma once

#include "../maths/Colour.hpp"
#include "../maths/Matrix4.hpp"
#include "../maths/Vector2.hpp"

#include "../renderer/pipelines/Descriptor.hpp"
#include "../renderer/pipelines/InputState.hpp"
#include "../renderer/pipelines/PipelineCreate.hpp"

namespace Flounder
{
	class ShaderWaters
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

			Colour diffuseColour;

			float shineDamper;
			float reflectivity;

			float ignoreReflections;
		};

		static DescriptorType typeUboScene;
		static DescriptorType typeUboObject;
	//	static DescriptorType samplerReflections;
		static Descriptor descriptor;
		static InputState inputState;

		static PipelineCreateInfo pipelineCreateInfo;
	};
}
