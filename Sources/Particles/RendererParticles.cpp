#include "RendererParticles.hpp"

#include "Maths/Maths.hpp"

namespace fl
{
	RendererParticles::RendererParticles(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformHandler()),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Particles/Particle.vert", "Resources/Shaders/Particles/Particle.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK), {}))
	{
	}

	RendererParticles::~RendererParticles()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererParticles::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		/*const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		UbosParticles::UboScene uboScene = {};
		uboScene.projection = camera.GetProjectionMatrix();
		uboScene.view = camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);*/

		/*for (auto iter = Particles::Get()->GetParticles()->begin(); iter != Particles::Get()->GetParticles()->end(); ++iter)
		{
			std::vector<UbosParticles::UboObject> objects = {};

			for (auto particle : *(*iter).second)
			{
				UbosParticles::UboObject object = {};
				object.transform = ModelMatrix(particle, camera.GetViewMatrix());
				object.textureOffsets.m_x = particle->GetTextureOffset1()->m_x;
				object.textureOffsets.m_y = particle->GetTextureOffset1()->m_y;
				object.textureOffsets.m_z = particle->GetTextureOffset2()->m_x;
				object.textureOffsets.m_w = particle->GetTextureOffset2()->m_y;
				object.blendFactor = particle->GetTextureBlendFactor();
				object.transparency = particle->GetTransparency();
				objects.push_back(object);
			}
		}*/
	}

	Matrix4 RendererParticles::ModelMatrix(Particle *particle, const Matrix4 &viewMatrix)
	{
		Matrix4 modelMatrix = modelMatrix.Translate(*particle->GetPosition());
		modelMatrix.m_00 = viewMatrix.m_00;
		modelMatrix.m_01 = viewMatrix.m_10;
		modelMatrix.m_02 = viewMatrix.m_20;
		modelMatrix.m_10 = viewMatrix.m_01;
		modelMatrix.m_11 = viewMatrix.m_11;
		modelMatrix.m_12 = viewMatrix.m_21;
		modelMatrix.m_20 = viewMatrix.m_02;
		modelMatrix.m_21 = viewMatrix.m_12;
		modelMatrix.m_22 = viewMatrix.m_22;
		modelMatrix = modelMatrix.Rotate(Maths::Radians(particle->GetRotation()), Vector3::FRONT);
		modelMatrix = modelMatrix.Scale(Vector3(particle->GetScale(), particle->GetScale(), particle->GetScale()));
		return Matrix4();
	}
}
