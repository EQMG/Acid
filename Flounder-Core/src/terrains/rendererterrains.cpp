#include "rendererterrains.h"

namespace flounder
{
	rendererterrains::rendererterrains() :
		irenderer()
	{
		m_shader = new shader("terrains", 2,
			shadertype(VERTEX, "res/shaders/terrains/terrainVertex.glsl", FILE),
			shadertype(FRAGMENT, "res/shaders/terrains/terrainFragment.glsl", FILE)
		);
	}

	rendererterrains::~rendererterrains()
	{
		delete m_shader;
	}

	void rendererterrains::render(const vector4 &clipPlane, const icamera &camera)
	{
		prepareRendering(clipPlane, camera);
		
		for (terrain *object : *terrains::get()->getTerrains())
		{
			renderTerrain(object);
		}

		endRendering();
	}

	void rendererterrains::prepareRendering(const vector4 &clipPlane, const icamera &camera)
	{
		// Starts the shader.
		m_shader->start();

		// Loads the uniforms.
		m_shader->loadUniform4fv("projectionMatrix", *camera.getProjectionMatrix());
		m_shader->loadUniform4fv("viewMatrix", *camera.getViewMatrix());
		m_shader->loadUniform4f("clipPlane", clipPlane);

		// Sets the GPU for rendering this object.
		renderer::get()->antialias(display::get()->isAntialiasing());
		renderer::get()->enableDepthTesting();
		renderer::get()->cullBackFaces(true);
	}

	void rendererterrains::renderTerrain(terrain *object)
	{
		// Binds the layouts.
		renderer::get()->bindVAO(object->getModel()->getVaoID(), 3, 0, 2, 3);

		// Loads the uniforms.
		m_shader->loadUniform4fv("modelMatrix", *object->getModelMatrix());

		m_shader->loadUniform1f("shineDamper", 1.0f);
		m_shader->loadUniform1f("reflectivity", 0.0f);

		// Tells the GPU to render this object.
#if 0
		renderer::get()->renderElements(GL_TRIANGLES, GL_UNSIGNED_INT, object->getModel()->getVaoLength());
#endif

		// Unbinds the layouts.
		renderer::get()->unbindVAO(3, 0, 2, 3);
	}

	void rendererterrains::endRendering()
	{
		// Stops the shader.
		m_shader->stop();
	}
}
