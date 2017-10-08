#pragma once

#include "../maths/Matrix4.hpp"

#include "../renderer/pipelines/Descriptor.hpp"
#include "../renderer/pipelines/InputState.hpp"
#include "../renderer/pipelines/PipelineCreate.hpp"

namespace Flounder
{
	class ShaderTerrains
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
			float shineDamper;
			float reflectivity;
		};

		static DescriptorType typeUboScene;
		static DescriptorType typeUboObject;
		static Descriptor descriptor;
		static InputState inputState;

		static PipelineCreateInfo pipelineCreateInfo;
	};
}
