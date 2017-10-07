#pragma once

#include "../maths/Vector2.hpp"
#include "../maths/Matrix4.hpp"

#include "../renderer/pipelines/Descriptor.hpp"
#include "../renderer/pipelines/InputState.hpp"
#include "../renderer/pipelines/PipelineCreate.hpp"

namespace Flounder
{
	class TestShader
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
			float swaying;
			Vector2 swayOffset;
		};

		static DescriptorType typeUboScene;
		static DescriptorType typeUboObject;
		static DescriptorType typeSamplerDiffuse;
		static DescriptorType typeSamplerSway;
		static Descriptor descriptor;
		static InputState inputState;

		static PipelineCreateInfo pipelineCreateInfo;
	};
}
