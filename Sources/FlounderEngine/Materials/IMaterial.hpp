#pragma once

#include "Scenes/Objects/Component.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "PipelineMaterial.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a material shader.
	/// </summary>
	class F_EXPORT IMaterial :
		public Component
	{
	public:
		IMaterial() :
			Component()
		{
		}

		virtual ~IMaterial()
		{
		}

		virtual void PushUniforms(UniformHandler *uniformObject) = 0;

		virtual void PushDescriptors(DescriptorsHandler *descriptorSet) = 0;

		std::string GetName() const override { return "IMaterial"; };

		virtual PipelineMaterial *GetMaterial() const = 0;
	};
}