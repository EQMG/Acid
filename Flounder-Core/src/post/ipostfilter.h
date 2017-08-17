#pragma once

#include <stdarg.h>

#include "../fbos/fbo.h"
#include "../models/model.h"
#include "../renderer/renderer.h"
#include "../shaders/shader.h"

namespace flounder
{
	/// <summary>
	/// Represents a post effect shader and on application saves the result into a fbo.
	/// </summary>
	class ipostfilter
	{
	protected:
		shader *m_shader;
		fbo *m_fbo;
		model *m_model;
	public:
		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="filterName"> The name for the filter. </param>
		/// <param name="fragmentShader"> The fragment shader file. </param>
		/// <param name="fbo"> The fbo to render into. </param>
		ipostfilter(const std::string &filterName, const std::string &fragmentShader, fbo *fbo);

		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="filterName"> The name for the filter. </param>
		/// <param name="fragmentShader"> The fragment shader file. </param>
		ipostfilter(const std::string &filterName, const std::string &fragmentShader);

		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="shader"> The shader for the filter. </param>
		/// <param name="fbo"> The fbo to render into. </param>
		ipostfilter(shader *shader, fbo *fbo);

		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="shader"> The shader for the filter. </param>
		ipostfilter(shader *shader);

		/// <summary>
		/// Deconstructor for the post filter.
		/// </summary>
		virtual ~ipostfilter();

		/// <summary>
		/// Renders the filter to its fbo.
		/// </summary>
		/// <param name="n_args"> The number textures being bound to the shader. </param>
		/// <param name="..."> The textures being bound to the shader. </param>
		void applyFilter(const int n_args, ...);

		/// <summary>
		/// Renders the filter to its fbo.
		/// </summary>
		/// <param name="n_args"> The number textures being bound to the shader. </param>
		/// <param name="..."> The textures being bound to the shader. </param>
		void applyFilter(const int n_args, va_list args);

		/// <summary>
		/// Can be used to store values into the shader, this is called when the filter is applied and the shader has been already started.
		/// </summary>
		virtual void storeValues() = 0;

		/// <summary>
		/// Gets the fbo the filter rendered into.
		/// </summary>
		/// <returns> The fbo. </returns>
		inline fbo *getFbo() const { return m_fbo; }
	};
}
