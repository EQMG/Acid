#include "deferredrenderer.h"

namespace flounder 
{
	deferredrenderer::deferredrenderer()
	{
		m_shader = shader::newShader()->addName("deferredRenderer")
			->addType(shadertype(GL_VERTEX_SHADER, "res/shaders/deferred/deferredVertex.glsl", loadtype::FILE))
			->addType(shadertype(GL_FRAGMENT_SHADER, "res/shaders/deferred/deferredFragment.glsl", loadtype::FILE))->create();
		m_fbo = fbo::newFBO()->fitToScreen(1.0f)->create();
		m_model = model::newModel()->setFile("res/models/filter.obj")->create();
	}

	deferredrenderer::~deferredrenderer()
	{
		delete m_shader;
		delete m_fbo;
	}

	void deferredrenderer::apply(const int n_args, ...)
	{
		bool lastWireframe = renderer::get()->isInWireframe();

		m_fbo->bindFrameBuffer();
		renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f, 1.0f);
		m_shader->start();

		storeValues();

		renderer::get()->antialias(false);
		renderer::get()->disableDepthTesting();
		renderer::get()->cullBackFaces(true);
		renderer::get()->goWireframe(false);
		renderer::get()->bindVAO(m_model->getVaoID(), 2, 0, 1);

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			GLuint texture = va_arg(ap, GLuint);
			renderer::get()->bindTexture(texture, GL_TEXTURE_2D, i);
		}

		va_end(ap);

		renderer::get()->renderElements(GL_TRIANGLES, GL_UNSIGNED_INT, m_model->getVaoLength()); // Render post filter.

		renderer::get()->unbindVAO(2, 0, 1);
		renderer::get()->goWireframe(lastWireframe);
		m_shader->stop();
		renderer::get()->disableBlending();
		renderer::get()->enableDepthTesting();
		m_fbo->unbindFrameBuffer();
	}

	void deferredrenderer::storeValues()
	{
		m_shader->loadUniform("projectionMatrix", camera::get()->getCamera()->getProjectionMatrix());
		m_shader->loadUniform("viewMatrix", camera::get()->getCamera()->getViewMatrix());

		int lightsLoaded = 0;

		/*if (entities::get()->getEntities() != 0)
		{
			for (unknown::const_iterator entity = FlounderEntities::get()->getEntities()->getAll(0).begin(); entity != FlounderEntities::get()->getEntities()->getAll(0).end(); ++entity)
			{
				ComponentLight *componentLight = static_cast<ComponentLight*>((*entity)->getComponent(ComponentLight::typeid));

				if (lightsLoaded < LIGHTS && componentLight != 0)
				{
					shader->getUniformBool("lightActive[" + std::to_string(lightsLoaded) + "]", true);
					shader->getUniformVec3("lightColour[" + std::to_string(lightsLoaded) + "]", componentLight->getLight()->getColour());
					shader->getUniformVec3("lightPosition[" + std::to_string(lightsLoaded) + "]", componentLight->getLight()->getPosition());
					shader->getUniformVec3("lightAttenuation[" + std::to_string(lightsLoaded) + "]", componentLight->getLight()->getAttenuation());
					lightsLoaded++;
				}
			}
		}*/

		if (lightsLoaded < LIGHTS)
		{
			for (int i = lightsLoaded; i < LIGHTS; i++)
			{
				m_shader->loadUniform("lightActive[" + std::to_string(i) + "]", false);
				m_shader->loadUniform("lightColour[" + std::to_string(i) + "]", 0.0f, 0.0f, 0.0f);
				m_shader->loadUniform("lightPosition[" + std::to_string(i) + "]", 0.0f, 0.0f, 0.0f);
				m_shader->loadUniform("lightAttenuation[" + std::to_string(i) + "]", 1.0f, 0.0f, 0.0f);
			}
		}

		/*m_shader->loadUniform("shadowSpaceMatrix", shadows::get()->getToShadowMapSpaceMatrix());
		m_shader->loadUniform("shadowDistance", shadows::get()->getShadowBoxDistance());
		m_shader->loadUniform("shadowTransition", shadows::get()->getShadowTransition());
		m_shader->loadUniform("shadowMapSize", shadows::get()->getShadowSize());
		m_shader->loadUniform("shadowPCF", shadows::get()->getShadowPCF());
		m_shader->loadUniform("shadowBias", shadows::get()->getShadowBias());
		m_shader->loadUniform("shadowDarkness", shadows::get()->getShadowDarkness() * shadows::get()->getShadowFactor());
		m_shader->loadUniform("brightnessBoost", shadows::get()->getBrightnessBoost());*/

		if (skybox::get()->getFog() != NULL)
		{
			m_shader->loadUniform("fogColour", skybox::get()->getFog()->m_colour);
			m_shader->loadUniform("fogDensity", skybox::get()->getFog()->m_density);
			m_shader->loadUniform("fogGradient", skybox::get()->getFog()->m_gradient);
		}
		else
		{
			m_shader->loadUniform("fogColour", 1.0f, 1.0f, 1.0f);
			m_shader->loadUniform("fogDensity", 0.003f);
			m_shader->loadUniform("fogGradient", 2.0f);
		}
	}
}