#include "rendererguis.h"

namespace flounder
{
	rendererguis::rendererguis() :
		irenderer()
	{
		m_shader = shader::newShader()->addName("skybox")
		                              ->addType(shadertype(GL_VERTEX_SHADER, "res/shaders/guis/guiVertex.glsl", loadtype::FILE))
		                              ->addType(shadertype(GL_FRAGMENT_SHADER, "res/shaders/guis/guiFragment.glsl", loadtype::FILE))
		                              ->create();
		std::vector<GLfloat> positions = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
		m_vaoID = loaders::get()->createVAO();
		loaders::get()->storeDataInVBO(m_vaoID, positions, 0, 2);
		m_vaoLength = positions.size() / 2;
	}

	rendererguis::~rendererguis()
	{
		delete m_shader;
		glDeleteVertexArrays(1, &m_vaoID);
	}

	void rendererguis::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);

		for (screenobject *object : *guis::get()->getObjects())
		{
			guiobject *gui = dynamic_cast<guiobject*>(object);

			if (gui != NULL)
			{
				renderGui(gui);
			}
		}

		endRendering();
	}

	void rendererguis::prepareRendering(const vector4 &clipPlane, const icamera &camera)
	{
		// Starts the shader.
		m_shader->start();

		// Binds the layouts.
		renderer::get()->bindVAO(m_vaoID, 1, 0);

		// Loads the uniforms.
		m_shader->loadUniform("aspectRatio", static_cast<float>(display::get()->getAspectRatio()));
		m_shader->loadUniform("polygonMode", renderer::get()->isInWireframe());

		// Sets the GPU for rendering this object.
		renderer::get()->antialias(false);
		renderer::get()->disableDepthTesting();
		renderer::get()->cullBackFaces(true);
		renderer::get()->enableAlphaBlending();
	}

	void rendererguis::renderGui(guiobject *object)
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
		m_shader->loadUniform("size", *object->getMeshSize());
		m_shader->loadUniform("transform",
		                      object->getScreenPosition()->m_x, object->getScreenPosition()->m_y,
		                      object->getScreenDimensions()->m_x, object->getScreenDimensions()->m_y
		);
		m_shader->loadUniform("rotation", static_cast<float>(__radians(object->getRotation())));

		m_shader->loadUniform("alpha", object->getAlpha());
		m_shader->loadUniform("flipTexture", object->getFlipTexture());
		m_shader->loadUniform("atlasRows", static_cast<float>(object->getTexture()->getNumberOfRows()));
		m_shader->loadUniform("atlasOffset", *object->getTextureOffset());
		m_shader->loadUniform("colourOffset", *object->getColourOffset());

		// Tells the GPU to render this object.
		renderer::get()->renderArrays(GL_TRIANGLE_STRIP, m_vaoLength);
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
