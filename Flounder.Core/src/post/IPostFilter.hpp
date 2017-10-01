#pragma once

#include "../fbos/Fbo.hpp"
#include "../models/Model.hpp"
#include "../renderer/Renderer.hpp"
#include "../shaders/Shader.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a post effect shader and on application saves the result into a fbo.
	/// </summary>
	class IPostFilter
	{
	protected:
		Shader *m_shader;
		Fbo *m_fbo;
		Model *m_model;
	public:
		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="filterName"> The name for the filter. </param>
		/// <param name="fragmentShader"> The fragment shader file. </param>
		/// <param name="fbo"> The fbo to render into. </param>
		IPostFilter(const std::string &filterName, const std::string &fragmentShader, Fbo *fbo = new Fbo(true, 1.0f));

		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="shader"> The shader for the filter. </param>
		/// <param name="fbo"> The fbo to render into. </param>
		IPostFilter(Shader *shader, Fbo *fbo);

		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="shader"> The shader for the filter. </param>
		IPostFilter(Shader *shader);

		/// <summary>
		/// Deconstructor for the post filter.
		/// </summary>
		virtual ~IPostFilter();

		/// <summary>
		/// Renders the filter to its fbo.
		/// </summary>
		/// <param name="n_args"> The number textures being bound to the shader. </param>
		/// <param name="..."> The textures being bound to the shader. </param>
		void ApplyFilter(const int n_args, ...);

		/// <summary>
		/// Renders the filter to its fbo.
		/// </summary>
		/// <param name="n_args"> The number textures being bound to the shader. </param>
		/// <param name="..."> The textures being bound to the shader. </param>
		void ApplyFilter(const int n_args, va_list args);

		/// <summary>
		/// Can be used to store values into the shader, this is called when the filter is applied and the shader has been already started.
		/// </summary>
		virtual void StoreValues() = 0;

		/// <summary>
		/// Gets the fbo the filter rendered into.
		/// </summary>
		/// <returns> The fbo. </returns>
		Fbo *GetFbo() const { return m_fbo; }
	};
}
