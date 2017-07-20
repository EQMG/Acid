#include "rendererskybox.h"

namespace flounder 
{
	rendererskybox::rendererskybox() :
		irenderer()
	{
		m_shader = shader::newShader()->addName("skybox")
			->addType(shadertype(GL_VERTEX_SHADER, "res/shaders/skybox/skyboxVertex.glsl", loadtype::FILE))
			->addType(shadertype(GL_FRAGMENT_SHADER, "res/shaders/skybox/skyboxFragment.glsl", loadtype::FILE))
			->create();
	}

	rendererskybox::~rendererskybox()
	{
		delete m_shader;
	}

	void rendererskybox::render(const vector4 &clipPlane, const icamera &camera)
	{
		// Starts the shader.
		m_shader->start();

		// Binds the layouts.
		renderer::get()->bindVAO(skybox::get()->getModel()->getVaoID(), 1, 0);
		renderer::get()->bindTexture(skybox::get()->getTexture(), 0);

		// Loads the uniforms.
		m_shader->loadUniform("projectionMatrix", *camera.getProjectionMatrix());
		m_shader->loadUniform("viewMatrix", *camera.getViewMatrix());
		m_shader->loadUniform("modelMatrix", *skybox::get()->getModelMatrix());
		m_shader->loadUniform("clipPlane", clipPlane);
		m_shader->loadUniform("polygonMode", renderer::get()->isInWireframe());
		m_shader->loadUniform("skyColour", *skybox::get()->getFog()->m_colour);
		m_shader->loadUniform("blendFactor", skybox::get()->getBlend());

		// Sets the GPU for rendering this object.
		renderer::get()->antialias(display::get()->isAntialiasing());
		renderer::get()->enableDepthTesting();
		renderer::get()->depthMask(false);
		renderer::get()->cullBackFaces(false);
		renderer::get()->disableBlending();

		// Tells the GPU to render this object.
		renderer::get()->renderElements(GL_TRIANGLES, GL_UNSIGNED_INT, skybox::get()->getModel()->getVaoLength());

		// Unbinds the layouts.
		renderer::get()->unbindVAO(1, 0);
		renderer::get()->depthMask(true);

		// Stops the shader.
		m_shader->stop();
	}
}
