#include "rendererfonts.h"

namespace flounder
{
	rendererfonts::rendererfonts() :
		irenderer()
	{
		m_shader = shader::newShader()->addName("fonts")
		                              ->addType(shadertype(GL_VERTEX_SHADER, "res/shaders/fonts/fontVertex.glsl", loadtype::FILE))
		                              ->addType(shadertype(GL_FRAGMENT_SHADER, "res/shaders/fonts/fontFragment.glsl", loadtype::FILE))
		                              ->create();
	}

	rendererfonts::~rendererfonts()
	{
		delete m_shader;
	}

	void rendererfonts::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);

		for (uiobject *screenobject : *uis::get()->getObjects())
		{
			text *object = dynamic_cast<text*>(screenobject);

			if (object != NULL)
			{
				renderText(object);
			}
		}

		endRendering();
	}

	void rendererfonts::prepareRendering(const vector4 &clipPlane, const icamera &camera)
	{
		// Starts the shader.
		m_shader->start();

		// Loads the uniforms.
		m_shader->loadUniform("aspectRatio", static_cast<float>(display::get()->getAspectRatio()));
		m_shader->loadUniform("polygonMode", renderer::get()->isInWireframe());

		// Sets the GPU for rendering this object.
		renderer::get()->antialias(false);
		renderer::get()->disableDepthTesting();
		renderer::get()->cullBackFaces(true);
		renderer::get()->enableAlphaBlending();
	}

	void rendererfonts::renderText(text *object)
	{
		// Binds the layouts.
		renderer::get()->bindVAO(object->getVaoID(), 2, 0, 1);
		renderer::get()->bindTexture(object->getTexture(), 0);

		// Applies the GL scissor test.
		vector4 *scissor = object->getScissor();

		if (scissor->m_z != -1.0f && scissor->m_w != -1.0f)
		{
			renderer::get()->scissorEnable(static_cast<int>(scissor->m_x), static_cast<int>(scissor->m_y), static_cast<int>(scissor->m_z), static_cast<int>(scissor->m_w));
		}

		// Loads the uniforms.
		m_shader->loadUniform("size", *object->getMeshSize());
		m_shader->loadUniform("transform",
		                      object->getScreenPosition()->m_x, object->getScreenPosition()->m_y,
		                      object->getScreenDimensions()->m_x, object->getScreenDimensions()->m_y
		);
		m_shader->loadUniform("rotation", static_cast<float>(__radians(object->getRotation())));

		m_shader->loadUniform("colour", object->getTextColour()->m_r, object->getTextColour()->m_g, object->getTextColour()->m_b, object->getAlpha());
		m_shader->loadUniform("borderColour", *object->getBorderColour());
		m_shader->loadUniform("edgeData", object->calculateEdgeStart(), object->calculateAntialiasSize());
		m_shader->loadUniform("borderSizes", object->getTotalBorderSize(), object->getGlowSize());

		// Tells the GPU to render this object.
		renderer::get()->renderArrays(GL_TRIANGLES, object->getVaoLength());
		renderer::get()->scissorDisable();

		// Unbinds the layouts.
		renderer::get()->unbindVAO(2, 0, 1);
	}

	void rendererfonts::endRendering()
	{
		// Stops the shader.
		m_shader->stop();
	}
}
