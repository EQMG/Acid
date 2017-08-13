#include "renderershadows.h"

namespace flounder
{
	renderershadows::renderershadows() :
		irenderer()
	{
		m_fbo = new fbo(shadows::get()->getShadowSize(), shadows::get()->getShadowSize(), TEXTURE, false);

		m_shader = new shader("shadows", 2,
			shadertype{ VERTEX, "res/shaders/shadows/shadow.vert.spv" },
			shadertype{ FRAGMENT, "res/shaders/shadows/shadow.frag.spv" }
		);
	}

	renderershadows::~renderershadows()
	{
		delete m_fbo;
		delete m_shader;
	}

	void renderershadows::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);

		for (terrain *object : *terrains::get()->getTerrains())
		{
			renderModel(object->getModel(), object->getModelMatrix());
		}

		endRendering();
	}

	void renderershadows::prepareRendering(const vector4 &clipPlane, const icamera &camera)
	{
#if 0
		m_fbo->setSize(shadows::get()->getShadowSize(), shadows::get()->getShadowSize());

		// Starts the shader.
		m_fbo->bindFrameBuffer();
		renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f, 1.0f);
		m_shader->start();

		// Sets the GPU for rendering this object.
		renderer::get()->enableDepthTesting();
		renderer::get()->depthMask(true);
		renderer::get()->cullBackFaces(false);
#endif
	}

	void renderershadows::renderModel(model *object, matrix4x4 *modelMatrix)
	{
#if 0
		// Binds the layouts.
		renderer::get()->bindVAO(object->getVaoID(), 1, 0);

		// Loads the uniforms.
		matrix4x4 *mvp = matrix4x4::multiply(*shadows::get()->getShadowBox()->getProjectionViewMatrix(), *modelMatrix, nullptr);
		m_shader->loadUniform4fv("mvpMatrix", *mvp);
		delete mvp;

		// Tells the GPU to render this object.
		renderer::get()->renderElements(GL_TRIANGLES, GL_UNSIGNED_INT, object->getVaoLength());

		// Unbinds the layouts.
		renderer::get()->unbindVAO(1, 0);
#endif
	}

	void renderershadows::endRendering()
	{
#if 0
		// Stops the shader.
		m_shader->stop();
		m_fbo->unbindFrameBuffer();
#endif
	}
}
