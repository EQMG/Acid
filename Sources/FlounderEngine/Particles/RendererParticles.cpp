#include "RendererParticles.hpp"

#include "../Maths/Maths.hpp"
#include "UbosParticles.hpp"

namespace Flounder
{
	RendererParticles::RendererParticles(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosParticles::UboScene))),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Particles/Particle.vert", "Resources/Shaders/Particles/Particle.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), {}))
	{
	}

	RendererParticles::~RendererParticles()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererParticles::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		/*const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		UbosParticles::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);*/

		/*for (auto iter = Particles::Get()->GetParticles()->begin(); iter != Particles::Get()->GetParticles()->end(); ++iter)
		{
			std::vector<UbosParticles::UboObject> objects = {};

			for (auto particle : *(*iter).second)
			{
				UbosParticles::UboObject object = {};
				object.transform = ModelMatrix(particle, *camera.GetViewMatrix());
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
		Matrix4 modelMatrix = Matrix4();
		Matrix4::Translate(modelMatrix, *particle->GetPosition(), &modelMatrix);
		modelMatrix.m_00 = viewMatrix.m_00;
		modelMatrix.m_01 = viewMatrix.m_10;
		modelMatrix.m_02 = viewMatrix.m_20;
		modelMatrix.m_10 = viewMatrix.m_01;
		modelMatrix.m_11 = viewMatrix.m_11;
		modelMatrix.m_12 = viewMatrix.m_21;
		modelMatrix.m_20 = viewMatrix.m_02;
		modelMatrix.m_21 = viewMatrix.m_12;
		modelMatrix.m_22 = viewMatrix.m_22;
		Matrix4::Rotate(modelMatrix, Vector3::FRONT, Maths::Radians(particle->GetRotation()), &modelMatrix);
		Matrix4::Scale(modelMatrix, Vector3(particle->GetScale(), particle->GetScale(), particle->GetScale()), &modelMatrix);
		return Matrix4();
	}
}
