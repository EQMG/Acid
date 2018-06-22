#pragma once

#include "Objects/IComponent.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "PipelineMaterial.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a material shader.
	/// </summary>
	class FL_EXPORT IMaterial :
		public IComponent
	{
	public:
		IMaterial() :
			IComponent()
		{
		}

		virtual ~IMaterial()
		{
		}

		virtual void Update() override = 0;

		virtual void Load(std::shared_ptr<LoadedValue> value) override = 0;

		virtual void Write(std::shared_ptr<LoadedValue> destination) override = 0;

		virtual void PushUniforms(UniformHandler &uniformObject) = 0;

		virtual void PushDescriptors(DescriptorsHandler &descriptorSet) = 0;

		virtual std::string GetName() const override = 0;

		virtual std::shared_ptr<PipelineMaterial> GetMaterial() const = 0;
	};
}