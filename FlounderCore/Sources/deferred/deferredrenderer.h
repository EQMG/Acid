#pragma once

#include "../camera/camera.h"
#include "../fbos/fbo.h"
#include "../lights/light.h"
#include "../loaders/loaders.h"
#include "../maths/vector3.h"
#include "../models/model.h"
#include "../renderer/renderer.h"
#include "../shaders/shader.h"
#include "../skybox/skybox.h"

namespace flounder {
	/// <summary>
	/// A filter-like shader/fbo used to process deferred rendering.
	/// </summary>
	class deferredrenderer
	{
	private:
		static const int LIGHTS = 64;

		shader *m_shader;
		fbo *m_fbo;
		model *m_model;
	public:
		deferredrenderer();

		~deferredrenderer();

		/// <summary>
		/// Processes the textures provided into the deferred renderer.
		/// </summary>
		/// <param name="n_args"> The number textures being bound to the shader. </param>
		/// <param name="..."> The textures being bound to the shader. Colours, normals, extras, depth, shadows. </param>
		void apply(const int n_args, ...);

		inline fbo *getFbo() { return m_fbo; }
	private:
		void storeValues();
	};
}
