#include "rendererfonts.h"

namespace flounder
{
	rendererfonts::rendererfonts() :
		irenderer()
	{
		m_shader = new shader("fonts", 2,
			shadertype{ VERTEX, "res/shaders/fonts/font.vert.spv" },
			shadertype{ FRAGMENT, "res/shaders/fonts/font.frag.spv" }
		);
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

			if (object != nullptr)
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
		m_shader->loadUniform1f("aspectRatio", static_cast<float>(display::get()->getAspectRatio()));
		m_shader->loadUniform1i("polygonMode", renderer::get()->isInWireframe());

		// Sets the GPU for rendering this object.
		renderer::get()->antialias(false);
		renderer::get()->disableDepthTesting();
		renderer::get()->cullBackFaces(true);
		renderer::get()->enableAlphaBlending();
	}

	void rendererfonts::renderText(text *object)
	{
		// Binds the layouts.
		renderer::get()->bindVAO(object->getModel()->getVaoID(), 2, 0, 1);
		renderer::get()->bindTexture(object->getTexture(), 0);

		// Applies the GL scissor test.
		vector4 *scissor = object->getScissor();

		if (scissor->m_z != -1.0f && scissor->m_w != -1.0f)
		{
			renderer::get()->scissorEnable(static_cast<int>(scissor->m_x), static_cast<int>(scissor->m_y), static_cast<int>(scissor->m_z), static_cast<int>(scissor->m_w));
		}

		// Loads the uniforms.
		m_shader->loadUniform2f("size", *object->getMeshSize());
		m_shader->loadUniform4f("transform",
		                      object->getScreenPosition()->m_x, object->getScreenPosition()->m_y,
		                      object->getScreenDimensions()->m_x, object->getScreenDimensions()->m_y
		);
		m_shader->loadUniform1f("rotation", static_cast<float>(__radians(object->getRotation())));

		m_shader->loadUniform1f("alpha", object->getAlpha()); 
		m_shader->loadUniform4f("colour", object->getTextColour()->m_r, object->getTextColour()->m_g, object->getTextColour()->m_b, object->getAlpha());
		m_shader->loadUniform3f("borderColour", *object->getBorderColour());
		m_shader->loadUniform2f("edgeData", object->calculateEdgeStart(), object->calculateAntialiasSize());
		m_shader->loadUniform2f("borderSizes", object->getTotalBorderSize(), object->getGlowSize());

		// Tells the GPU to render this object.
#if 0
		renderer::get()->renderArrays(GL_TRIANGLES, object->getModel()->getVaoLength());
#endif
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
