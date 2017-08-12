#include "rendererskyboxes.h"

namespace flounder
{
	rendererskyboxes::rendererskyboxes() :
		irenderer()
	{
		m_shader = new shader("skyboxes", 2,
			shadertype{ VERTEX, "res/shaders/skyboxes/skybox.vert.spv" },
			shadertype{ FRAGMENT, "res/shaders/skyboxes/skybox.frag.spv" }
		);
	}

	rendererskyboxes::~rendererskyboxes()
	{
		delete m_shader;
	}

	void rendererskyboxes::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);
		renderSkybox(skyboxes::get()->getSkybox());
		endRendering();
	}

	void rendererskyboxes::prepareRendering(const vector4 &clipPlane, const icamera &camera)
	{
		// Starts the shader.
		m_shader->start();

		// Loads the uniforms.
		m_shader->loadUniform4fv("projectionMatrix", *camera.getProjectionMatrix());
		m_shader->loadUniform4fv("viewMatrix", *camera.getViewMatrix());
		m_shader->loadUniform4f("clipPlane", clipPlane);

		// Sets the GPU for rendering this object.
		renderer::get()->enableDepthTesting();
		renderer::get()->depthMask(false);
		renderer::get()->cullBackFaces(false);
		renderer::get()->disableBlending();
	}

	void rendererskyboxes::renderSkybox(skybox *object)
	{
		// Binds the layouts.
		renderer::get()->bindVAO(object->getModel()->getVaoID(), 1, 0);
		renderer::get()->bindTexture(object->getTexture(), 0);

		// Loads the uniforms.
		m_shader->loadUniform4fv("modelMatrix", *object->getModelMatrix());
		m_shader->loadUniform3f("skyColour", *skyboxes::get()->getFog()->m_colour);
		m_shader->loadUniform1f("blendFactor", object->getBlend());

		// Tells the GPU to render this object.
#if 0
		renderer::get()->renderElements(GL_TRIANGLES, GL_UNSIGNED_INT, object->getModel()->getVaoLength());
#endif

		// Unbinds the layouts.
		renderer::get()->unbindVAO(1, 0);
	}

	void rendererskyboxes::endRendering()
	{
		// Unbinds the layouts.
		renderer::get()->depthMask(true);

		// Stops the shader.
		m_shader->stop();
	}
}
