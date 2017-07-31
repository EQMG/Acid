#include "rendererentities.h"

namespace flounder
{
	rendererentities::rendererentities() :
		irenderer()
	{
#ifdef FLOUNDER_PLATFORM_WEB
		m_shader = shader::newShader()->addName("entities")
			->addType(shadertype(GL_VERTEX_SHADER, "res/shaders/entities/entityVertex.web.glsl", loadtype::FILE))
			->addType(shadertype(GL_FRAGMENT_SHADER, "res/shaders/entities/entityFragment.web.glsl", loadtype::FILE))
			->create();
#else
		m_shader = shader::newShader()->addName("entities")
		                              ->addType(shadertype(GL_VERTEX_SHADER, "res/shaders/entities/entityVertex.glsl", loadtype::FILE))
		                              ->addType(shadertype(GL_FRAGMENT_SHADER, "res/shaders/entities/entityFragment.glsl", loadtype::FILE))
		                              ->create();
#endif
	}

	rendererentities::~rendererentities()
	{
		delete m_shader;
	}

	void rendererentities::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);

		for (entity *object: *entities::get()->getEntities())
		{
			renderEntity(object);
		}

		endRendering();
	}

	void rendererentities::prepareRendering(const vector4 &clipPlane, const icamera &camera)
	{
		// Starts the shader.
		m_shader->start();

		// Binds the layouts.
		/*renderer::get()->bindVAO(m_vaoID, 1, 0);

		// Loads the uniforms.
		m_shader->loadUniform("aspectRatio", static_cast<float>(display::get()->getAspectRatio()));
		m_shader->loadUniform("polygonMode", renderer::get()->isInWireframe());

		// Sets the GPU for rendering this object.
		renderer::get()->antialias(false);
		renderer::get()->disableDepthTesting();
		renderer::get()->cullBackFaces(true);
		renderer::get()->enableAlphaBlending();*/
	}

	void rendererentities::renderEntity(entity *object)
	{
		/*// Binds the layouts.
		renderer::get()->bindTexture(object->getTexture(), 0);

		// Loads the uniforms.
		m_shader->loadUniform("size", *object->getMeshSize());
		m_shader->loadUniform("transform",
		                      object->getScreenPosition()->m_x, object->getScreenPosition()->m_y,
		                      object->getScreenDimensions()->m_x, object->getScreenDimensions()->m_y
		);
		m_shader->loadUniform("rotation", static_cast<float>(__radians(object->getRotation())));

		m_shader->loadUniform("alpha", object->getAlpha());
		m_shader->loadUniform("flipX", object->getFlipX());
		m_shader->loadUniform("flipY", object->getFlipY());
		m_shader->loadUniform("atlasRows", static_cast<float>(object->getTexture()->getNumberOfRows()));
		m_shader->loadUniform("atlasOffset", *object->getTextureOffset());
		m_shader->loadUniform("colourOffset", *object->getColourOffset());

		// Tells the GPU to render this object.
		renderer::get()->renderArrays(GL_TRIANGLE_STRIP, m_vaoLength);*/
	}

	void rendererentities::endRendering()
	{
		// Unbinds the layouts.
		//renderer::get()->unbindVAO(1, 0);

		// Stops the shader.
		m_shader->stop();
	}
}
