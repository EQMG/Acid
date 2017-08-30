#include "rendererterrains.hpp"

namespace flounder
{
	rendererterrains::rendererterrains() :
		irenderer(),
		m_shader(new shader("terrains", 2,
			shadertype(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/terrains/terrain.vert.spv"),
			shadertype(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/terrains/terrain.frag.spv")
		))
	{
	}

	rendererterrains::~rendererterrains()
	{
		delete m_shader;
	}

	void rendererterrains::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);

		for (auto object : *terrains::get()->getTerrains())
		{
			renderTerrain(object);
		}

		endRendering();
	}

	void rendererterrains::prepareRendering(const vector4 &clipPlane, const icamera &camera)
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

	void rendererterrains::renderTerrain(terrain *object)
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

	void rendererterrains::endRendering()
	{
#if 0
		// Stops the shader.
		m_shader->stop();
#endif
	}
}
