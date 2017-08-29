#pragma once

#include "../camera/camera.hpp"
#include "../fbos/fbo.hpp"
#include "../lights/light.hpp"
#include "../maths/vector3.hpp"
#include "../models/model.hpp"
#include "../renderer/renderer.hpp"
#include "../shaders/shader.hpp"
#include "../skyboxes/skyboxes.hpp"
#include "../shadows/shadows.hpp"
#include "../worlds/worlds.hpp"

namespace flounder
{
	/// <summary>
	/// A filter-like shader/fbo used to process deferred rendering.
	/// </summary>
	class rendererdeferred
	{
	private:
		static const int LIGHTS = 64;

		shader *m_shader;
		fbo *m_fbo;
		model *m_model;
	public:
		rendererdeferred(fbo *fbo);

		/// <summary>
		/// Creates a new deferred renderer.
		/// </summary>
		rendererdeferred();

		/// <summary>
		/// Deconstructor for the deferred renderer.
		/// </summary>
		~rendererdeferred();

		/// <summary>
		/// Processes the textures provided into the deferred renderer.
		/// </summary>
		/// <param name="n_args"> The number textures being bound to the shader. </param>
		/// <param name="..."> The textures being bound to the shader. Colours, normals, extras, depth, shadows. </param>
		void apply(const int n_args, ...);

		/// <summary>
		/// Gets the fbo the filter rendered into.
		/// </summary>
		/// <returns> The fbo. </returns>
		fbo *getFbo() const { return m_fbo; }
	private:
		/// <summary>
		/// Can be used to store values into the shader, this is called when the filter is applied and the shader has been already started.
		/// </summary>
		void storeValues();
	};
}
