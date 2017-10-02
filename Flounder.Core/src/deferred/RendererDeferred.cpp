#include "RendererDeferred.hpp"

#include "../camera/Camera.hpp"
#include "../lights/Light.hpp"
#include "../maths/Vector3.hpp"
#include "../renderer/Renderer.hpp"
#include "../skyboxes/Skyboxes.hpp"
#include "../shadows/Shadows.hpp"
#include "../worlds/Worlds.hpp"

namespace Flounder
{
	RendererDeferred::RendererDeferred(Fbo *fbo) :
		m_shader(new Shader("deferred", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/deferred/deferred.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/deferred/deferred.frag.spv")
		})),
		m_model(new Model("res/models/rectangle_flat.obj"))
	{
	}

	RendererDeferred::RendererDeferred() :
		RendererDeferred(new Fbo(true, 1.0f))
	{
	}

	RendererDeferred::~RendererDeferred()
	{
		delete m_shader;
		delete m_fbo;
	}

	void RendererDeferred::Apply(const int n_args, ...)
	{
#if 0
		m_fbo->bindFrameBuffer();
		renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f, 1.0f);
		m_shader->start();

		storeValues();

		renderer::get()->disableDepthTesting();
		renderer::get()->cullBackFaces(true);
		renderer::get()->bindVAO(m_model->getVaoID(), 2, 0, 1);

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			int texture = va_arg(ap, int);
			renderer::get()->bindTexture(texture, GL_TEXTURE_2D, i);
		}

		va_end(ap);

		renderer::get()->renderElements(GL_TRIANGLES, GL_UNSIGNED_INT, m_model->getVaoLength()); // Render post filter.

		renderer::get()->unbindVAO(2, 0, 1);
		m_shader->stop();
		renderer::get()->disableBlending();
		renderer::get()->enableDepthTesting();
		m_fbo->unbindFrameBuffer();
#endif
	}

	void RendererDeferred::StoreValues()
	{
#if 0
		m_shader->loadUniform4fv("projectionMatrix", *camera::get()->getCamera()->getProjectionMatrix());
		m_shader->loadUniform4fv("viewMatrix", *camera::get()->getCamera()->getViewMatrix());

		int lightsLoaded = 0;

		/*if (entities::get()->getEntities() != 0)
		{
			for (unknown::const_iterator entity = FlounderEntities::get()->getEntities()->getAll(0).begin(); entity != FlounderEntities::get()->getEntities()->getAll(0).end(); ++entity)
			{
				ComponentLight *componentLight = static_cast<ComponentLight*>((*entity)->getComponent(ComponentLight::typeid));

				if (lightsLoaded < LIGHTS && componentLight != 0)
				{
					m_shader->loadUniform1i("lightActive[" + std::to_string(lightsLoaded) + "]", true);
					m_shader->loadUniform3f("lightColour[" + std::to_string(lightsLoaded) + "]", *componentLight->getLight()->getColour());
					m_shader->loadUniform3f("lightPosition[" + std::to_string(lightsLoaded) + "]", *componentLight->getLight()->getPosition());
					m_shader->loadUniform3f("lightAttenuation[" + std::to_string(lightsLoaded) + "]", *componentLight->getLight()->getAttenuation());
					lightsLoaded++;
				}
			}
		}*/

		m_shader->loadUniform1i("lightActive[0]", true);
		m_shader->loadUniform3f("lightColour[0]", *worlds::get()->getSunColour());
		m_shader->loadUniform3f("lightPosition[0]", *worlds::get()->getSunPosition());
		m_shader->loadUniform3f("lightAttenuation[0]", 1.0f, 0.0f, 0.0f);
		lightsLoaded = 1;

		if (lightsLoaded < LIGHTS)
		{
			for (int i = lightsLoaded; i < LIGHTS; i++)
			{
				m_shader->loadUniform1i("lightActive[" + std::to_string(i) + "]", false);
				m_shader->loadUniform3f("lightColour[" + std::to_string(i) + "]", 0.0f, 0.0f, 0.0f);
				m_shader->loadUniform3f("lightPosition[" + std::to_string(i) + "]", 0.0f, 0.0f, 0.0f);
				m_shader->loadUniform3f("lightAttenuation[" + std::to_string(i) + "]", 1.0f, 0.0f, 0.0f);
			}
		}

		m_shader->loadUniform4fv("shadowSpaceMatrix", *shadows::get()->getShadowBox()->getToShadowMapSpaceMatrix());
		m_shader->loadUniform1f("shadowDistance", shadows::get()->getShadowBoxDistance());
		m_shader->loadUniform1f("shadowTransition", shadows::get()->getShadowTransition());
		m_shader->loadUniform1i("shadowMapSize", shadows::get()->getShadowSize());
		m_shader->loadUniform1i("shadowPCF", shadows::get()->getShadowPCF());
		m_shader->loadUniform1f("shadowBias", shadows::get()->getShadowBias());
		m_shader->loadUniform1f("shadowDarkness", shadows::get()->getShadowDarkness() * shadows::get()->getShadowFactor());
		m_shader->loadUniform1f("brightnessBoost", shadows::get()->getBrightnessBoost());

		if (skyboxes::get()->getFog() != nullptr)
		{
			m_shader->loadUniform3f("fogColour", *skyboxes::get()->getFog()->m_colour);
			m_shader->loadUniform1f("fogDensity", skyboxes::get()->getFog()->m_density);
			m_shader->loadUniform1f("fogGradient", skyboxes::get()->getFog()->m_gradient);
		}
		else
		{
			m_shader->loadUniform3f("fogColour", 1.0f, 1.0f, 1.0f);
			m_shader->loadUniform1f("fogDensity", 0.003f);
			m_shader->loadUniform1f("fogGradient", 2.0f);
		}
#endif
	}
}
