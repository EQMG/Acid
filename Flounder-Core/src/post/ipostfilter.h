#pragma once

#include "../fbos/fbo.h"
#include "../loaders/loaders.h"
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
		ipostfilter(const std::string &filterName, const std::string &fragmentShader);

		ipostfilter(const std::string &filterName, const std::string &fragmentShader, fbo *fbo);

		ipostfilter(shader *shader);

		ipostfilter(shader *shader, fbo *fbo);

		~ipostfilter();

		/// <summary>
		/// Renders the filter to its fbo.
		/// </summary>
		/// <param name="n_args"> The number textures being bound to the shader. </param>
		/// <param name="..."> The textures being bound to the shader. </param>
		void applyFilter(const int n_args, ...);

		/// <summary>
		/// Can be used to store values into the shader, this is called when the filter is applied and the shader has been already started.
		/// </summary>
		virtual void storeValues() = 0;

		inline fbo *getFbo() { return m_fbo; }
	};
}
