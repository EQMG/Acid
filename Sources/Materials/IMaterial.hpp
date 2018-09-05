#pragma once

#include "Objects/IComponent.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "PipelineMaterial.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a material shader.
	/// </summary>
	class ACID_EXPORT IMaterial :
		public IComponent
	{
	public:
		virtual void Start() override = 0;

		virtual void Update() override = 0;

		virtual void Decode(const Metadata &metadata) override = 0;

		virtual void Encode(Metadata &metadata) const override = 0;

		virtual void PushUniforms(UniformHandler &uniformObject) = 0;

		virtual void PushDescriptors(DescriptorsHandler &descriptorSet) = 0;

		virtual std::shared_ptr<PipelineMaterial> GetMaterial() const = 0;
	};
}
