#include "rendererwaters.hpp"

namespace Flounder
{
	rendererwaters::rendererwaters() : 
		irenderer(),
		m_fboReflection(new Fbo(true, waters::get()->getReflectionQuality(), DepthTexture, true, 3)),
		m_rendererDeferred(new rendererdeferred()),
		m_shader(new shader("waters", 2,
			shadertype(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/waters/water.vert.spv"),
			shadertype(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/waters/water.frag.spv")
		))
	{
	}

	rendererwaters::~rendererwaters()
	{
		delete m_fboReflection;
		delete m_rendererDeferred;

		delete m_shader;
	}

	void rendererwaters::render(const vector4 &clipPlane, const ICamera &camera)
	{
		prepareRendering(clipPlane, camera);
		renderWater(waters::get()->getWater());
		endRendering();
	}

	void rendererwaters::prepareRendering(const vector4 &clipPlane, const ICamera &camera)
	{
#if 0
		// Starts the shader.
		m_shader->start();

		// Loads the uniforms.
		m_shader->loadUniform4fv("projectionMatrix", *camera.getProjectionMatrix());
		m_shader->loadUniform4fv("viewMatrix", *camera.getViewMatrix());
		m_shader->loadUniform4f("clipPlane", clipPlane);

		if (waters::get()->getEnableReflections() && waters::get()->getColourIntensity() != 1.0f)
		{
		// Update the quality scalar.
			if (m_fboReflection->getSizeScalar() != waters::get()->getReflectionQuality())
			{
				m_fboReflection->setSizeScalar(waters::get()->getReflectionQuality());
			}

		// Binds the reflection FBO.
			if (m_rendererDeferred != nullptr)
			{
				renderer::get()->bindTexture(m_rendererDeferred->getFbo()->getColourTexture(0), GL_TEXTURE_2D, 0);
			}
			else
			{
				renderer::get()->bindTexture(m_fboReflection->getColourTexture(0), GL_TEXTURE_2D, 0);
			}
		}

		// Sets the GPU for rendering this object.
		renderer::get()->enableDepthTesting();
		renderer::get()->cullBackFaces(true);
#endif
	}

	void rendererwaters::renderWater(water *object)
	{
#if 0
		// Binds the layouts.
		renderer::get()->bindVAO(object->getModel()->getVaoID(), 1, 0);

		// Loads the uniforms.
		m_shader->loadUniform4fv("modelMatrix", *object->getModelMatrix());

		m_shader->loadUniform3f("waterOffset", *object->getOffset());

		 m_shader->loadUniform4f("diffuseColour", 
			 object->getColour()->m_r, object->getColour()->m_g, object->getColour()->m_b, 
			 waters::get()->getEnableReflections() ? waters::get()->getColourIntensity() : 1.0f
		 );

		m_shader->loadUniform1f("waveTime", Engine::Get()->getTimeSec() / water::WAVE_SPEED);
		m_shader->loadUniform1f("waveLength", water::WAVE_LENGTH);
		m_shader->loadUniform1f("amplitude", water::AMPLITUDE);
		m_shader->loadUniform1f("squareSize", static_cast<float>(water::SQUARE_SIZE));
		m_shader->loadUniform1f("waterHeight", object->getPosition()->m_y);

		m_shader->loadUniform1f("shineDamper", water::SHINE_DAMPER);
		m_shader->loadUniform1f("reflectivity", water::REFLECTIVITY);

		m_shader->loadUniform1i("ignoreReflections", !waters::get()->getEnableReflections());

		// Tells the GPU to render this object.
#if 0
		renderer::get()->renderArrays(GL_TRIANGLES, object->getModel()->getVaoLength());
#endif

		// Unbinds the layouts.
		renderer::get()->unbindVAO(1, 0);
#endif
	}

	void rendererwaters::endRendering()
	{
#if 0
		// Stops the shader.
		m_shader->stop();
#endif
	}
}
