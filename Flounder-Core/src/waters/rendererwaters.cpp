#include "rendererwaters.h"

namespace flounder
{
	rendererwaters::rendererwaters() :
		irenderer()
	{
		m_fboReflection = fbo::newFBO()->fitToScreen(waters::get()->getReflectionQuality())->attachments(3)->withAlphaChannel(true)->depthBuffer(TEXTURE)->create();
		m_rendererDeferred = new rendererdeferred(fbo::newFBO()->fitToScreen(1.0f)->disableTextureWrap()->create());

		m_shader = shader::newShader()->addName("waters")
			->addType(shadertype(GL_VERTEX_SHADER, "res/shaders/waters/waterVertex.glsl", loadtype::FILE))
			->addType(shadertype(GL_FRAGMENT_SHADER, "res/shaders/waters/waterFragment.glsl", loadtype::FILE))
			->create();
	}

	rendererwaters::~rendererwaters()
	{
		delete m_fboReflection;
		delete m_rendererDeferred;

		delete m_shader;
	}

	void rendererwaters::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);
		renderWater(waters::get()->getWater());
		endRendering();
	}

	void rendererwaters::prepareRendering(const vector4 &clipPlane, const icamera &camera)
	{
		// Starts the shader.
		m_shader->start();

		// Loads the uniforms.
		m_shader->loadUniform4fv("projectionMatrix", *camera.getProjectionMatrix());
		m_shader->loadUniform4fv("viewMatrix", *camera.getViewMatrix());
		m_shader->loadUniform4f("clipPlane", clipPlane);

		/*m_shader->loadUniform3f("waterOffset",
			2.0f * water::SQUARE_SIZE * round(camera.getPosition()->m_x / (2.0f * water::SQUARE_SIZE)),
			0.0f,
			2.0f * water::SQUARE_SIZE * round(camera.getPosition()->m_z / (2.0f * water::SQUARE_SIZE))
		);*/

		if (waters::get()->getEnableReflections() && waters::get()->getColourIntensity() != 1.0f)
		{
			// Update the quality scalar.
			if (m_fboReflection->getSizeScalar() != waters::get()->getReflectionQuality())
			{
				m_fboReflection->setSizeScalar(waters::get()->getReflectionQuality());
			}

			// Binds the reflection FBO.
			if (m_rendererDeferred != NULL)
			{
				renderer::get()->bindTexture(m_rendererDeferred->getFbo()->getColourTexture(0), GL_TEXTURE_2D, 0);
			}
			else
			{
				renderer::get()->bindTexture(m_fboReflection->getColourTexture(0), GL_TEXTURE_2D, 0);
			}
		}

		// Sets the GPU for rendering this object.
		renderer::get()->antialias(display::get()->isAntialiasing());
		renderer::get()->enableDepthTesting();
		renderer::get()->cullBackFaces(true);
	}

	void rendererwaters::renderWater(water *object)
	{
		// Binds the layouts.
		renderer::get()->bindVAO(object->getVaoID(), 1, 0);

		// Loads the uniforms.
		m_shader->loadUniform4fv("modelMatrix", *object->getModelMatrix());

		 m_shader->loadUniform4f("diffuseColour", object->getColour()->m_r, object->getColour()->m_g, object->getColour()->m_b, waters::get()->getEnableReflections() ? waters::get()->getColourIntensity() : 1.0f);
		//m_shader->loadUniform4f("diffuseColour", *object->getColour());

		m_shader->loadUniform1f("waveTime", framework::get()->getTimeSec() / water::WAVE_SPEED);
		m_shader->loadUniform1f("waveLength", water::WAVE_LENGTH);
		m_shader->loadUniform1f("amplitude", water::AMPLITUDE);
		m_shader->loadUniform1f("squareSize", static_cast<float>(water::SQUARE_SIZE));
		m_shader->loadUniform1f("waterHeight", object->getPosition()->m_y);

		m_shader->loadUniform1f("shineDamper", water::SHINE_DAMPER);
		m_shader->loadUniform1f("reflectivity", water::REFLECTIVITY);

		m_shader->loadUniform1i("ignoreReflections", !waters::get()->getEnableReflections());

		// Tells the GPU to render this object.
		renderer::get()->renderArrays(GL_TRIANGLES, object->getVaoLength());

		// Unbinds the layouts.
		renderer::get()->unbindVAO(1, 0);
	}

	void rendererwaters::endRendering()
	{
		// Stops the shader.
		m_shader->stop();
	}
}
