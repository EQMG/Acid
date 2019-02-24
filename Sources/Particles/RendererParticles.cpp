#include "RendererParticles.hpp"

#include "Scenes/Scenes.hpp"
#include "Models/VertexModel.hpp"
#include "Particles.hpp"

namespace acid
{
	RendererParticles::RendererParticles(const Pipeline::Stage &pipelineStage) :
		RenderPipeline(pipelineStage),
		m_pipeline(pipelineStage, {"Shaders/Particles/Particle.vert", "Shaders/Particles/Particle.frag"}, {VertexModel::GetVertexInput(0), ParticleType::GetVertexInput(1)},
			PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::Read, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, false, {})
	{
	}

	void RendererParticles::Render(const CommandBuffer &commandBuffer)
	{
		auto camera = Scenes::Get()->GetCamera();
		m_uniformScene.Push("projection", camera->GetProjectionMatrix());
		m_uniformScene.Push("view", camera->GetViewMatrix());

		auto particles = Particles::Get()->GetParticles();

		m_pipeline.BindPipeline(commandBuffer);

		for (auto &[type, typeParticles] : particles)
		{
			type->CmdRender(commandBuffer, m_pipeline, m_uniformScene, typeParticles);
		}
	}
}
