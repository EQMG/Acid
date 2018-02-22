#include "RendererParticles.hpp"

#include "../Maths/Maths.hpp"

namespace Flounder
{
	const int RendererParticles::MAX_INSTANCES = 27500;
	const int RendererParticles::INSTANCE_DATA_LENGTH = 22;

	RendererParticles::RendererParticles(const int &subpass) :
		IRenderer(),
		//	m_shader(new Shader("particles", {
		//		ShaderStage(VK_SHADER_STAGE_VERTEX_BIT, "Resources/Shaders/Particles/Particle.vert.spv"),
		//	ShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, "Resources/Shaders/Particles/Particle.frag.spv")
		//	})),
		m_rendered(0)
	{
		/*std::vector<float> positions = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};
		m_vaoID = loaders::get()->createVAO();
		loaders::get()->storeDataInVBO(m_vaoID, positions, 0, 2);
		m_vboID = loaders::get()->createEmptyVBO(INSTANCE_DATA_LENGTH * MAX_INSTANCES);
		m_vaoLength = (int) positions.size();

		loaders::get()->addInstancedAttribute(m_vaoID, m_vboID, 1, 4, INSTANCE_DATA_LENGTH, 0);
		loaders::get()->addInstancedAttribute(m_vaoID, m_vboID, 2, 4, INSTANCE_DATA_LENGTH, 4);
		loaders::get()->addInstancedAttribute(m_vaoID, m_vboID, 3, 4, INSTANCE_DATA_LENGTH, 8);
		loaders::get()->addInstancedAttribute(m_vaoID, m_vboID, 4, 4, INSTANCE_DATA_LENGTH, 12);
		loaders::get()->addInstancedAttribute(m_vaoID, m_vboID, 5, 4, INSTANCE_DATA_LENGTH, 16);
		loaders::get()->addInstancedAttribute(m_vaoID, m_vboID, 6, 1, INSTANCE_DATA_LENGTH, 20);
		loaders::get()->addInstancedAttribute(m_vaoID, m_vboID, 7, 1, INSTANCE_DATA_LENGTH, 21);*/
	}

	RendererParticles::~RendererParticles()
	{
#if 0
		glDeleteBuffers(1, &m_vboID);
		glDeleteVertexArrays(1, &m_vaoID);
#endif
	}

	void RendererParticles::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		PrepareRendering(clipPlane, camera);

		for (auto iter = Particles::Get()->GetParticles()->begin();
			 iter != Particles::Get()->GetParticles()->end(); ++iter)
		{
			std::vector<float> *vboData = new std::vector<float>();
			m_rendered = 0;

			for (auto it = iter->second->begin(); it != iter->second->end(); ++it)
			{
				PrepareInstance(*it, camera, vboData);
			}

			RenderInstances((*iter).first, vboData);
			delete vboData;
		}

		EndRendering();
	}

	void RendererParticles::PrepareRendering(const Vector4 &clipPlane, const ICamera &camera)
	{
#if 0
		// Starts the shader.
		m_shader->start();

		// Loads the uniforms.
		m_shader->loadUniform4fv("projectionMatrix", *camera.getProjectionMatrix());
		m_shader->loadUniform4fv("viewMatrix", *camera.getViewMatrix());
		m_shader->loadUniform4f("clipPlane", clipPlane);

		// Sets the GPU for rendering this object.
		renderer::get()->enableDepthTesting();
		// renderer::get()->depthMask(false);
		renderer::get()->cullBackFaces(true);
		renderer::get()->enableAlphaBlending();
#endif
	}

	void RendererParticles::PrepareInstance(Particle *particle, const ICamera &camera, std::vector<float> *vboData)
	{
		if (m_rendered >= MAX_INSTANCES)
		{
			printf("Particles overflow: %i\n", m_rendered);
			return;
		}

		Matrix4 *viewMatrix = camera.GetViewMatrix();
		Matrix4 *modelMatrix = new Matrix4();
		Matrix4::Translate(*modelMatrix, *particle->GetTransform()->GetPosition(), modelMatrix);
		modelMatrix->m_00 = viewMatrix->m_00;
		modelMatrix->m_01 = viewMatrix->m_10;
		modelMatrix->m_02 = viewMatrix->m_20;
		modelMatrix->m_10 = viewMatrix->m_01;
		modelMatrix->m_11 = viewMatrix->m_11;
		modelMatrix->m_12 = viewMatrix->m_21;
		modelMatrix->m_20 = viewMatrix->m_02;
		modelMatrix->m_21 = viewMatrix->m_12;
		modelMatrix->m_22 = viewMatrix->m_22;
		Matrix4::Rotate(*modelMatrix, Vector3(0.0f, 0.0f, 1.0f), Maths::Radians(particle->GetRotation()), modelMatrix);
		Matrix4::Scale(*modelMatrix, Vector3(particle->GetScale(), particle->GetScale(), particle->GetScale()), modelMatrix);

		vboData->push_back(modelMatrix->m_00);
		vboData->push_back(modelMatrix->m_01);
		vboData->push_back(modelMatrix->m_02);
		vboData->push_back(modelMatrix->m_03);
		vboData->push_back(modelMatrix->m_10);
		vboData->push_back(modelMatrix->m_11);
		vboData->push_back(modelMatrix->m_12);
		vboData->push_back(modelMatrix->m_13);
		vboData->push_back(modelMatrix->m_20);
		vboData->push_back(modelMatrix->m_21);
		vboData->push_back(modelMatrix->m_22);
		vboData->push_back(modelMatrix->m_23);
		vboData->push_back(modelMatrix->m_30);
		vboData->push_back(modelMatrix->m_31);
		vboData->push_back(modelMatrix->m_32);
		vboData->push_back(modelMatrix->m_33);
		vboData->push_back(particle->GetTextureOffset1()->m_x);
		vboData->push_back(particle->GetTextureOffset1()->m_y);
		vboData->push_back(particle->GetTextureOffset2()->m_x);
		vboData->push_back(particle->GetTextureOffset2()->m_y);
		vboData->push_back(particle->GetTextureBlendFactor());
		vboData->push_back(particle->GetTransparency());

		delete modelMatrix;

		m_rendered++;
	}

	void RendererParticles::RenderInstances(ParticleType *particleType, std::vector<float> *vboData)
	{
#if 0
		loaders::get()->updateVBO(m_vboID, INSTANCE_DATA_LENGTH * MAX_INSTANCES, *vboData);
		renderer::get()->bindVAO(m_vaoID, 8, 0, 1, 2, 3, 4, 5, 6, 7);

		if (particleType->getTexture() != nullptr)
		{
			m_shader->loadUniform1f("numberOfRows", static_cast<float>(particleType->getTexture()->getNumberOfRows()));
			renderer::get()->bindTexture(particleType->getTexture(), 0);
		}

		renderer::get()->renderInstanced(GL_TRIANGLE_STRIP, m_vaoLength, m_rendered);

		// renderer::get()->depthMask(true);
		renderer::get()->disableBlending();
		renderer::get()->unbindVAO(8, 0, 1, 2, 3, 4, 5, 6, 7);
#endif
	}

	void RendererParticles::EndRendering()
	{
#if 0
		// Stops the shader.
		m_shader->stop();
#endif
	}
}
