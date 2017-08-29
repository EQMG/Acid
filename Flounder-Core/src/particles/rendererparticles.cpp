#include "rendererparticles.hpp"

namespace flounder
{
	const int rendererparticles::MAX_INSTANCES = 27500;
	const int rendererparticles::INSTANCE_DATA_LENGTH = 22;

	rendererparticles::rendererparticles() :
		irenderer()
	{
		m_shader = new shader("particles", 2,
			shadertype(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/particles/particle.vert.spv"),
			shadertype(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/particles/particle.frag.spv")
		);
		std::vector<float> positions = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};
		/*m_vaoID = loaders::get()->createVAO();
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

		m_rendered = 0;
	}

	rendererparticles::~rendererparticles()
	{
		delete m_shader;
#if 0
		glDeleteBuffers(1, &m_vboID);
		glDeleteVertexArrays(1, &m_vaoID);
#endif
	}

	void rendererparticles::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);

		for (std::map<particletype*, std::vector<particle*>*>::iterator iter = particles::get()->getParticles()->begin(); iter != particles::get()->getParticles()->end(); ++iter)
		{
			std::vector<float> *vboData = new std::vector<float>();
			m_rendered = 0;

			for (std::vector<particle*>::iterator it = iter->second->begin(); it != iter->second->end(); ++it)
			{
				prepareInstance(*it, camera, vboData);
			}

			renderInstances((*iter).first, vboData);
			delete vboData;
		}

		endRendering();
	}

	void rendererparticles::prepareRendering(const vector4 &clipPlane, const icamera &camera)
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

	void rendererparticles::prepareInstance(particle *particle, const icamera &camera, std::vector<float> *vboData)
	{
		if (m_rendered >= MAX_INSTANCES)
		{
			std::cerr << "Particles overflow: " << m_rendered << std::endl;
			return;
		}

		matrix4x4 *viewMatrix = camera.getViewMatrix();
		matrix4x4 *modelMatrix = new matrix4x4();
		matrix4x4::translate(*modelMatrix, *particle->getPosition(), modelMatrix);
		modelMatrix->m_00 = viewMatrix->m_00;
		modelMatrix->m_01 = viewMatrix->m_10;
		modelMatrix->m_02 = viewMatrix->m_20;
		modelMatrix->m_10 = viewMatrix->m_01;
		modelMatrix->m_11 = viewMatrix->m_11;
		modelMatrix->m_12 = viewMatrix->m_21;
		modelMatrix->m_20 = viewMatrix->m_02;
		modelMatrix->m_21 = viewMatrix->m_12;
		modelMatrix->m_22 = viewMatrix->m_22;
		matrix4x4::rotate(*modelMatrix, vector3(0.0f, 0.0f, 1.0f), __radians(particle->getRotation()), modelMatrix);
		matrix4x4::scale(*modelMatrix, vector3(particle->getScale(), particle->getScale(), particle->getScale()), modelMatrix);

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
