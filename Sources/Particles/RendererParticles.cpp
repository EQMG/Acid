#include "RendererParticles.hpp"

#include "Maths/Maths.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	RendererParticles::RendererParticles(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_uniformScene(UniformHandler()),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Particles/Particle.vert", "Shaders/Particles/Particle.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_READ, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {{"MAX_INSTANCES", String::To(ParticleType::MAX_TYPE_INSTANCES)}})))
	{
	}

	void RendererParticles::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());

		auto particles = Particles::Get()->GetParticles();

		m_pipeline.BindPipeline(commandBuffer);

		for (auto &[type, particles] : particles)
		{
			auto instanceData = std::vector<ParticleData>();
			instanceData.resize(ParticleType::MAX_TYPE_INSTANCES);
			uint32_t i = 0;

			for (auto &particle : particles)
			{
				if (!camera.GetViewFrustum().SphereInFrustum(particle.GetPosition(), particle.GetScale()))
				{
					continue;
				}

				instanceData[i] = GetInstanceData(particle, camera.GetViewMatrix());
				i++;

				if (i > instanceData.size())
				{
					break;
				}
			}

			type->CmdRender(commandBuffer, m_pipeline, m_uniformScene, instanceData);
		}
	}

	ParticleData RendererParticles::GetInstanceData(Particle &particle, const Matrix4 &viewMatrix)
	{
		ParticleData instanceData = {};

		instanceData.transform = Matrix4::TransformationMatrix(particle.GetPosition(), particle.GetRotation() * Vector3::FRONT, particle.GetScale() * Vector3::ONE);

		instanceData.colourOffset = particle.GetParticleType()->GetColourOffset();

		Vector4 offsets = Vector4();
		offsets.m_x = particle.GetTextureOffset1().m_x;
		offsets.m_y = particle.GetTextureOffset1().m_y;
		offsets.m_z = particle.GetTextureOffset2().m_x;
		offsets.m_w = particle.GetTextureOffset2().m_y;
		instanceData.offsets = offsets;

		Vector3 blend = Vector3();
		blend.m_x = particle.GetTextureBlendFactor();
		blend.m_y = particle.GetTransparency();
		blend.m_z = static_cast<float>(particle.GetParticleType()->GetNumberOfRows());
		instanceData.blend = blend;

		return instanceData;
	}
}
