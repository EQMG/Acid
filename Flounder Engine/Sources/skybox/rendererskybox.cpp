#include "rendererskybox.h"

namespace flounder {
	rendererskybox::rendererskybox()
	{
		m_shader = shader::newShader()->addName("skybox")
			->addType(shadertype(GL_VERTEX_SHADER, "Resources/shaders/skybox/skyboxVertex.glsl", loadtype::FILE))
			->addType(shadertype(GL_FRAGMENT_SHADER, "Resources/shaders/skybox/skyboxFragment.glsl", loadtype::FILE))
			->create();
	}

	rendererskybox::~rendererskybox()
	{
		delete m_shader;
	}

	void rendererskybox::render(vector4 *clipPlane, icamera *camera)
	{
		m_shader->start();
		m_shader->loadUniform("projectionMatrix", camera->getProjectionMatrix());
		m_shader->loadUniform("viewMatrix", camera->getViewMatrix());
		m_shader->loadUniform("modelMatrix", skybox::get()->getModelMatrix());
		m_shader->loadUniform("clipPlane", clipPlane);
		m_shader->loadUniform("polygonMode", renderer::get()->isInWireframe());
		m_shader->loadUniform("skyColour", skybox::get()->getFog()->m_colour);
		m_shader->loadUniform("blendFactor", skybox::get()->getBlend());

		renderer::get()->antialias(display::get()->isAntialiasing());
		renderer::get()->enableDepthTesting();
		renderer::get()->depthMask(false);
		renderer::get()->cullBackFaces(false);
		renderer::get()->disableBlending();

		renderer::get()->bindVAO(skybox::get()->getModel()->getVaoID(), 1, 0);
		renderer::get()->bindTexture(skybox::get()->getTexture(), 0);

		renderer::get()->renderElements(GL_TRIANGLES, GL_UNSIGNED_INT, skybox::get()->getModel()->getVaoLength());

		renderer::get()->unbindVAO(1, 0);
		renderer::get()->depthMask(true);

		m_shader->stop();
	}
}
