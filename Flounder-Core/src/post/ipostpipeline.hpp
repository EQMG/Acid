#pragma once

#include <stdarg.h>

#include "../fbos/fbo.hpp"

namespace flounder
{
	/// <summary>
	/// Represents a system of post effects.
	/// </summary>
	class ipostpipeline
	{
	public:
		/// <summary>
		/// Creates a new post pipeline.
		/// </summary>
		ipostpipeline();

		/// <summary>
		/// Deconstructor for the post pipeline.
		/// </summary>
		virtual ~ipostpipeline();

		/// <summary>
		/// Renders the post pipeline.
		/// </summary>
		/// <param name="n_args"> The number textures being sent to the pipeline. </param>
		/// <param name="..."> A list of textures in indexed order to be bound for the shader program. </param>
		void renderPipelineV(const int n_args, ...);

		/// <summary>
		/// Renders the post pipeline.
		/// </summary>
		/// <param name="n_args"> The number textures being sent to the pipeline. </param>
		/// <param name="args"> A list of textures in indexed order to be bound for the shader program. </param>
		virtual void renderPipeline(const int n_args, va_list args) = 0;

		/// <summary>
		/// Gets the fbo containing the pipeline output.
		/// </summary>
		/// <returns> The output fbo. </returns>
		virtual Fbo *getOutput() = 0;
	};
}
