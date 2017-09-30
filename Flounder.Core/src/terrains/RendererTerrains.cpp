#include "RendererTerrains.hpp"

namespace Flounder
{
	RendererTerrains::RendererTerrains() :
		IRenderer(),
		m_shader(new Shader("terrains", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/terrains/terrain.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/terrains/terrain.frag.spv")
		}))
	{
	}

	RendererTerrains::~RendererTerrains()
	{
		delete m_shader;
	}

	void RendererTerrains::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		PrepareRendering(clipPlane, camera);

		for (auto object : *Terrains::get()->GetTerrains())
		{
			RenderTerrain(object);
		}

		EndRendering();
	}

	void RendererTerrains::PrepareRendering(const Vector4 &clipPlane, const ICamera &camera)
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
		renderer::get()->cullBackFaces(true);
#endif
	}

	void RendererTerrains::RenderTerrain(Terrain *object)
	{
#if 0
		// Binds the layouts.
		renderer::get()->bindVAO(object->getModel()->getVaoID(), 3, 0, 2, 3);

		// Loads the uniforms.
		m_shader->loadUniform4fv("modelMatrix", *object->getModelMatrix());

		m_shader->loadUniform1f("shineDamper", 1.0f);
		m_shader->loadUniform1f("reflectivity", 0.0f);

		// Tells the GPU to render this object.
		renderer::get()->renderElements(GL_TRIANGLES, GL_UNSIGNED_INT, object->getModel()->getVaoLength());

		// Unbinds the layouts.
		renderer::get()->unbindVAO(3, 0, 2, 3);
#endif
	}

	void RendererTerrains::EndRendering()
	{
#if 0
		// Stops the shader.
		m_shader->stop();
#endif
	}
}
