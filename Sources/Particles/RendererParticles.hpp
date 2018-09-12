#pragma once

#include <vector>
#include "Models/Model.hpp"
#include "Renderer/Buffers/InstanceBuffer.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Particles.hpp"

namespace acid
{
	struct ParticleData
	{
		Vector4 mvp0;
		Vector4 mvp1;
		Vector4 mvp2;
		Vector4 mvp3;
		Colour colourOffset;
		Vector4 offsets;
		Vector3 blend;
	};

	class ACID_EXPORT RendererParticles :
		public IRenderer
	{
	private:
		static const uint32_t MAX_INSTANCES;

		std::shared_ptr<Model> m_model;
		UniformHandler m_uniformScene;
		DescriptorsHandler m_descriptorSet;
		InstanceBuffer m_instanceBuffer;
		Pipeline m_pipeline;
	public:
		RendererParticles(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		ParticleData GetInstanceData(Particle &particle, const Matrix4 &viewMatrix);

		static VertexInput GetVertexInput();
	};
}
