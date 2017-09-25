#include "RendererSkyboxes.hpp"

namespace Flounder
{
	RendererSkyboxes::RendererSkyboxes() :
		IRenderer(),
		m_shader(new Shader("skyboxes", 2,
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/skyboxes/skybox.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/skyboxes/skybox.frag.spv")
		))
	{
	}

	RendererSkyboxes::~RendererSkyboxes()
	{
		delete m_shader;
	}

	void RendererSkyboxes::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		PrepareRendering(clipPlane, camera);
		RenderSkybox(Skyboxes::Get()->GetSkybox());
		EndRendering();
	}

	void RendererSkyboxes::PrepareRendering(const Vector4 &clipPlane, const ICamera &camera)
	{
#if 0
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
#endif
	}

	void RendererSkyboxes::RenderSkybox(Skybox *object)
	{
#if 0
		// Binds the layouts.
		renderer::get()->bindVAO(object->getModel()->getVaoID(), 1, 0);
		renderer::get()->bindTexture(object->getTexture(), 0);

		// Loads the uniforms.
		m_shader->loadUniform4fv("modelMatrix", *object->getModelMatrix());
		m_shader->loadUniform3f("skyColour", *skyboxes::get()->getFog()->m_colour);
		m_shader->loadUniform1f("blendFactor", object->getBlend());

		// Tells the GPU to render this object.
		renderer::get()->renderElements(GL_TRIANGLES, GL_UNSIGNED_INT, object->getModel()->getVaoLength());

		// Unbinds the layouts.
		renderer::get()->unbindVAO(1, 0);
#endif
	}

	void RendererSkyboxes::EndRendering()
	{
#if 0
		// Unbinds the layouts.
		renderer::get()->depthMask(true);

		// Stops the shader.
		m_shader->stop();
#endif
	}
}
