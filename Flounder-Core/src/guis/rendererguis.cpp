#include "rendererguis.h"

namespace flounder
{
	rendererguis::rendererguis() :
		irenderer()
	{
		m_shader = new shader("guis", 2,
			shadertype(VERTEX, "res/shaders/guis/guiVertex.glsl", FILE),
			shadertype(FRAGMENT, "res/shaders/guis/guiFragment.glsl", FILE)
		);
		std::vector<float> *vertices = new std::vector<float>{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f };
		m_model = new model(nullptr, vertices, nullptr, nullptr, nullptr);
		delete vertices;
	}

	rendererguis::~rendererguis()
	{
		delete m_shader;
		
		delete m_model;
	}

	void rendererguis::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);

		for (uiobject *screenobject : *uis::get()->getObjects())
		{
			gui *object = dynamic_cast<gui*>(screenobject);

			if (object != nullptr)
			{
				renderGui(object);
			}
		}

		endRendering();
	}

	void rendererguis::prepareRendering(const vector4 &clipPlane, const icamera &camera)
	{
		// Starts the shader.
		m_shader->start();

		// Binds the layouts.
		renderer::get()->bindVAO(m_model->getVaoID(), 1, 0);

		// Loads the uniforms.
		m_shader->loadUniform1f("aspectRatio", static_cast<float>(display::get()->getAspectRatio()));
		m_shader->loadUniform1i("polygonMode", renderer::get()->isInWireframe());

		// Sets the GPU for rendering this object.
		renderer::get()->antialias(false);
		renderer::get()->disableDepthTesting();
		renderer::get()->cullBackFaces(true);
		renderer::get()->enableAlphaBlending();
	}

	void rendererguis::renderGui(gui *object)
	{
		// Binds the layouts.
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
		m_shader->loadUniform1i("flipTexture", object->getFlipTexture());
		m_shader->loadUniform1f("atlasRows", static_cast<float>(object->getTexture()->getNumberOfRows()));
		m_shader->loadUniform2f("atlasOffset", *object->getTextureOffset());
		m_shader->loadUniform3f("colourOffset", *object->getColourOffset());

		// Tells the GPU to render this object.
#if 0
		renderer::get()->renderArrays(GL_TRIANGLE_STRIP, m_model->getVaoLength());
#endif
		renderer::get()->scissorDisable();
	}

	void rendererguis::endRendering()
	{
		// Unbinds the layouts.
		renderer::get()->unbindVAO(1, 0);

		// Stops the shader.
		m_shader->stop();
	}
}
