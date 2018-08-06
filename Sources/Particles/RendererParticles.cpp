#include "RendererParticles.hpp"

#include "Maths/Maths.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	RendererParticles::RendererParticles(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_uniformScene(UniformHandler()),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Particles/Particle.vert", "Shaders/Particles/Particle.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {})))
	{
	}

	RendererParticles::~RendererParticles()
	{
	}

	void RendererParticles::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		/*auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto descriptorSet = m_pipeline->GetDescriptorSet();

		UbosParticles::UboScene uboScene = {};
		uboScene.projection = camera.GetProjectionMatrix();
		uboScene.view = camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);*/

		/*for (auto iter = Particles::Get()->GetParticles()->begin(); iter != Particles::Get()->GetParticles()->end(); ++iter)
		{
			std::vector<UbosParticles::UboObject> objects = {};

			for (auto &particle : *(*iter).second)
			{
				UbosParticles::UboObject object = {};
				object.transform = ModelMatrix(particle, camera.GetViewMatrix());
				object.textureOffsets.m_x = particle->GetTextureOffset1()->m_x;
				object.textureOffsets.m_y = particle->GetTextureOffset1()->m_y;
				object.textureOffsets.m_z = particle->GetTextureOffset2()->m_x;
				object.textureOffsets.m_w = particle->GetTextureOffset2()->m_y;
				object.blendFactor = particle->GetTextureBlendFactor();
				object.transparency = particle->GetTransparency();
				objects.emplace_back(object);
			}
		}*/
	}

	Matrix4 RendererParticles::ModelMatrix(Particle *particle, const Matrix4 &viewMatrix)
	{
		Matrix4 modelMatrix = modelMatrix.Translate(particle->GetPosition());
		modelMatrix[0][0] = viewMatrix[0][0];
		modelMatrix[0][1] = viewMatrix[1][0];
		modelMatrix[0][2] = viewMatrix[2][0];
		modelMatrix[1][0] = viewMatrix[0][1];
		modelMatrix[1][1] = viewMatrix[1][1];
		modelMatrix[1][2] = viewMatrix[2][1];
		modelMatrix[2][0] = viewMatrix[0][2];
		modelMatrix[2][1] = viewMatrix[1][2];
		modelMatrix[2][2] = viewMatrix[2][2];
		modelMatrix = modelMatrix.Rotate(Maths::Radians(particle->GetRotation()), Vector3::FRONT);
		modelMatrix = modelMatrix.Scale(Vector3(particle->GetScale(), particle->GetScale(), particle->GetScale()));
		return Matrix4();
	}
}
