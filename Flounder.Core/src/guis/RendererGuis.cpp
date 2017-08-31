#include "rendererguis.hpp"

namespace Flounder
{
	rendererguis::rendererguis() :
		irenderer(),
		m_shader(new shader("guis", 2,
			shadertype(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/guis/gui.vert.spv"),
			shadertype(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/guis/gui.frag.spv")
		)),
		m_model(new model(nullptr, &std::vector<float>{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f}, nullptr, nullptr, nullptr))
	{
	}

	rendererguis::~rendererguis()
	{
		delete m_shader;

		delete m_model;
	}

	void rendererguis::render(const Vector4 &clipPlane, const ICamera &camera)
	{
		prepareRendering(clipPlane, camera);

		for (auto screenobject : *uis::get()->getObjects())
		{
			gui *object = dynamic_cast<gui*>(screenobject);

			if (object != nullptr)
			{
				renderGui(object);
			}
		}

		endRendering();
	}

	void rendererguis::prepareRendering(const Vector4 &clipPlane, const ICamera &camera)
	{
#if 0
		// Starts the shader.
		m_shader->start();

		// Binds the layouts.
		renderer::get()->bindVAO(m_model->getVaoID(), 1, 0);

		// Loads the uniforms.
		m_shader->loadUniform1f("aspectRatio", static_cast<float>(Display::Get()->getAspectRatio()));

		// Sets the GPU for rendering this object.
		renderer::get()->disableDepthTesting();
		renderer::get()->cullBackFaces(true);
		renderer::get()->enableAlphaBlending();
#endif
	}

	void rendererguis::renderGui(gui *object)
	{
#if 0
		// Binds the layouts.
		renderer::get()->bindTexture(object->getTexture(), 0);

		// Applies the GL scissor test.
		Vector4 *scissor = object->getScissor();

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
		m_shader->loadUniform1f("rotation", static_cast<float>(Maths::Radians(object->getRotation())));

		m_shader->loadUniform1f("alpha", object->getAlpha());
		m_shader->loadUniform1i("flipTexture", object->getFlipTexture());
		m_shader->loadUniform1f("atlasRows", static_cast<float>(object->getTexture()->getNumberOfRows()));
		m_shader->loadUniform2f("atlasOffset", *object->getTextureOffset());
		m_shader->loadUniform3f("colourOffset", *object->getColourOffset());

		// Tells the GPU to render this object.
		renderer::get()->renderArrays(GL_TRIANGLE_STRIP, m_model->getVaoLength());
		renderer::get()->scissorDisable();
#endif
	}

	void rendererguis::endRendering()
	{
#if 0
		// Unbinds the layouts.
		renderer::get()->unbindVAO(1, 0);

		// Stops the shader.
		m_shader->stop();
#endif
	}
}
