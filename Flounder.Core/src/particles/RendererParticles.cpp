#include "rendererparticles.hpp"

namespace Flounder
{
	const int rendererparticles::MAX_INSTANCES = 27500;
	const int rendererparticles::INSTANCE_DATA_LENGTH = 22;

	rendererparticles::rendererparticles() :
		IRenderer(),
		m_shader(new shader("particles", 2,
			shadertype(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/particles/particle.vert.spv"),
			shadertype(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/particles/particle.frag.spv")
		)),
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

	rendererparticles::~rendererparticles()
	{
		delete m_shader;
#if 0
		glDeleteBuffers(1, &m_vboID);
		glDeleteVertexArrays(1, &m_vaoID);
#endif
	}

	void rendererparticles::Render(const Vector4 &clipPlane, const ICamera &camera)
	{
		prepareRendering(clipPlane, camera);

		for (auto iter = particles::get()->getParticles()->begin(); iter != particles::get()->getParticles()->end(); ++iter)
		{
			std::vector<float> *vboData = new std::vector<float>();
			m_rendered = 0;

			for (auto it = iter->second->begin(); it != iter->second->end(); ++it)
			{
				prepareInstance(*it, camera, vboData);
			}

			renderInstances((*iter).first, vboData);
			delete vboData;
		}

		endRendering();
	}

	void rendererparticles::prepareRendering(const Vector4 &clipPlane, const ICamera &camera)
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

	void rendererparticles::prepareInstance(particle *particle, const ICamera &camera, std::vector<float> *vboData)
	{
		if (m_rendered >= MAX_INSTANCES)
		{
			std::cerr << "Particles overflow: " << m_rendered << std::endl;
			return;
		}

		Matrix4 *viewMatrix = camera.GetViewMatrix();
		Matrix4 *modelMatrix = new Matrix4();
		Matrix4::translate(*modelMatrix, *particle->getPosition(), modelMatrix);
		modelMatrix->m_00 = viewMatrix->m_00;
		modelMatrix->m_01 = viewMatrix->m_10;
		modelMatrix->m_02 = viewMatrix->m_20;
		modelMatrix->m_10 = viewMatrix->m_01;
		modelMatrix->m_11 = viewMatrix->m_11;
		modelMatrix->m_12 = viewMatrix->m_21;
		modelMatrix->m_20 = viewMatrix->m_02;
		modelMatrix->m_21 = viewMatrix->m_12;
		modelMatrix->m_22 = viewMatrix->m_22;
		Matrix4::rotate(*modelMatrix, Vector3(0.0f, 0.0f, 1.0f), Maths::Radians(particle->getRotation()), modelMatrix);
		Matrix4::scale(*modelMatrix, Vector3(particle->getScale(), particle->getScale(), particle->getScale()), modelMatrix);

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
		vboData->push_back(particle->getTextureOffset1()->m_x);
		vboData->push_back(particle->getTextureOffset1()->m_y);
		vboData->push_back(particle->getTextureOffset2()->m_x);
		vboData->push_back(particle->getTextureOffset2()->m_y);
		vboData->push_back(particle->getTextureBlendFactor());
		vboData->push_back(particle->getTransparency());

		delete modelMatrix;

		m_rendered++;
	}

	void rendererparticles::renderInstances(particletype *particleType, std::vector<float> *vboData)
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

	void rendererparticles::endRendering()
	{
#if 0
		// Stops the shader.
		m_shader->stop();
#endif
	}
}
