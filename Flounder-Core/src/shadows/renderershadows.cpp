#include "renderershadows.h"

namespace flounder
{
	renderershadows::renderershadows() :
		irenderer()
	{
		m_fbo = fbo::newFBO()->setSize(shadows::get()->getShadowSize(), shadows::get()->getShadowSize())->noColourBuffer()->disableTextureWrap()->depthBuffer(TEXTURE)->create();

		m_shader = shader::newShader()->addName("shadows")
			->addType(shadertype(GL_VERTEX_SHADER, "res/shaders/shadows/shadowVertex.glsl", loadtype::FILE))
			->addType(shadertype(GL_FRAGMENT_SHADER, "res/shaders/shadows/shadowFragment.glsl", loadtype::FILE))
			->create();
	}

	renderershadows::~renderershadows()
	{
		delete m_fbo;
		delete m_shader;
	}

	void renderershadows::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);
	//	renderEntity(waters::get()->getWater());
		endRendering();
	}

	void renderershadows::prepareRendering(const vector4 &clipPlane, const icamera &camera)
	{
		m_fbo->setSize(shadows::get()->getShadowSize(), shadows::get()->getShadowSize());

		// Starts the shader.
		m_fbo->bindFrameBuffer();
		renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f, 1.0f);
		m_shader->start();

		// Sets the GPU for rendering this object.
		renderer::get()->antialias(display::get()->isAntialiasing());
		renderer::get()->enableDepthTesting();
		renderer::get()->cullBackFaces(false);
	}

	/*void renderershadows::renderEntity(entity *object)
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
	}*/

	void renderershadows::endRendering()
	{
		// Stops the shader.
		m_shader->stop();
		m_fbo->unbindFrameBuffer();
	}
}
