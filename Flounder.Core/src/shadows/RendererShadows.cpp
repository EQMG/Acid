#include "renderershadows.hpp"

namespace Flounder
{
	renderershadows::renderershadows() :
		irenderer(),
		m_fbo(new Fbo(shadows::get()->getShadowSize(), shadows::get()->getShadowSize(), DepthTexture, false)),
		m_shader(new shader("shadows", 2,
			shadertype(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/shadows/shadow.vert.spv"),
			shadertype(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/shadows/shadow.frag.spv")
		))
	{
	}

	renderershadows::~renderershadows()
	{
		delete m_fbo;
		delete m_shader;
	}

	void renderershadows::render(const Vector4 &clipPlane, const ICamera &camera)
	{
		prepareRendering(clipPlane, camera);

		for (auto object : *terrains::get()->getTerrains())
		{
			renderModel(object->getModel(), object->getModelMatrix());
		}

		endRendering();
	}

	void renderershadows::prepareRendering(const Vector4 &clipPlane, const ICamera &camera)
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

	void renderershadows::renderModel(model *object, Matrix4 *modelMatrix)
	{
#if 0
		// Binds the layouts.
		renderer::get()->bindVAO(object->getVaoID(), 1, 0);

		// Loads the uniforms.
		Matrix4 *mvp = Matrix4::multiply(*shadows::get()->getShadowBox()->getProjectionViewMatrix(), *modelMatrix, nullptr);
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
