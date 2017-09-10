#include "RendererShadows.hpp"

namespace Flounder
{
	RendererShadows::RendererShadows() :
		IRenderer(),
		m_fbo(new Fbo(Shadows::Get()->GetShadowSize(), Shadows::Get()->GetShadowSize(), DepthTexture, false)),
		m_shader(new Shader("shadows", 2,
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/shadows/shadow.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/shadows/shadow.frag.spv")
		))
	{
	}

	RendererShadows::~RendererShadows()
	{
		delete m_fbo;
		delete m_shader;
	}

	void RendererShadows::Render(const Vector4 &clipPlane, const ICamera &camera)
	{
		PrepareRendering(clipPlane, camera);

		for (auto object : *Terrains::get()->GetTerrains())
		{
			RenderModel(object->GetModel(), object->GetModelMatrix());
		}

		EndRendering();
	}

	void RendererShadows::PrepareRendering(const Vector4 &clipPlane, const ICamera &camera)
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

	void RendererShadows::RenderModel(Model *object, Matrix4 *modelMatrix)
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

	void RendererShadows::EndRendering()
	{
#if 0
		// Stops the shader.
		m_shader->stop();
		m_fbo->unbindFrameBuffer();
#endif
	}
}
