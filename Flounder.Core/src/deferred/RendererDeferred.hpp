#pragma once

#include "../camera/Camera.hpp"
#include "../fbos/Fbo.hpp"
#include "../lights/Light.hpp"
#include "../maths/Vector3.hpp"
#include "../models/Model.hpp"
#include "../renderer/Renderer.hpp"
#include "../shaders/Shader.hpp"
#include "../skyboxes/Skyboxes.hpp"
#include "../shadows/Shadows.hpp"
#include "../worlds/Worlds.hpp"

namespace Flounder
{
	/// <summary>
	/// A filter-like shader/fbo used to process deferred rendering.
	/// </summary>
	class RendererDeferred
	{
	private:
		static const int LIGHTS = 64;

		shader *m_shader;
		Fbo *m_fbo;
		model *m_model;
	public:
		RendererDeferred(Fbo *fbo);

		/// <summary>
		/// Creates a new deferred renderer.
		/// </summary>
		RendererDeferred();

		/// <summary>
		/// Deconstructor for the deferred renderer.
		/// </summary>
		~RendererDeferred();

		/// <summary>
		/// Processes the textures provided into the deferred renderer.
		/// </summary>
		/// <param name="n_args"> The number textures being bound to the shader. </param>
		/// <param name="..."> The textures being bound to the shader. Colours, normals, extras, depth, shadows. </param>
		void Apply(const int n_args, ...);

		/// <summary>
		/// Gets the fbo the filter rendered into.
		/// </summary>
		/// <returns> The fbo. </returns>
		Fbo *GetFbo() const { return m_fbo; }
	private:
		/// <summary>
		/// Can be used to store values into the shader, this is called when the filter is applied and the shader has been already started.
		/// </summary>
		void StoreValues();
	};
}
