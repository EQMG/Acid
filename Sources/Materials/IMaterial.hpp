#pragma once

#include "Objects/Component.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "PipelineMaterial.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a material shader.
	/// </summary>
	class FL_EXPORT IMaterial :
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

		virtual void Update() override = 0;

		virtual void Load(LoadedValue *value) override = 0;

		virtual void Write(LoadedValue *destination) override = 0;

		virtual void PushUniforms(UniformHandler *uniformObject) = 0;

		virtual void PushDescriptors(DescriptorsHandler *descriptorSet) = 0;

		virtual std::string GetName() const override { return "IMaterial"; };

		virtual PipelineMaterial *GetMaterial() const = 0;
	};
}