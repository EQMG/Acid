#include "SubrenderParticles.hpp"

#include "Scenes/Scenes.hpp"
#include "Models/VertexDefault.hpp"
#include "Particles.hpp"

namespace acid
{
SubrenderParticles::SubrenderParticles(const Pipeline::Stage &pipelineStage) :
	Subrender(pipelineStage),
	m_pipeline(pipelineStage, {"Shaders/Particles/Particle.vert", "Shaders/Particles/Particle.frag"},
		{VertexDefault::GetVertexInput(0), ParticleType::Instance::GetVertexInput(1)}, {}, 
		PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::Read, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
{
}

void SubrenderParticles::Render(const CommandBuffer &commandBuffer)
{
	auto camera = Scenes::Get()->GetCamera();
	m_uniformScene.Push("projection", camera->GetProjectionMatrix());
	m_uniformScene.Push("view", camera->GetViewMatrix());

	m_pipeline.BindPipeline(commandBuffer);

	auto particles = Particles::Get()->GetParticles();

	for (auto &[type, typeParticles] : particles)
	{
		type->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
	}
}
}
