#pragma once

#include "../fbos/fbo.h"

namespace flounder {
	/// <summary>
	/// Represents a system of post effects.
	/// </summary>
	class ipostpipeline
	{
	public:
		/// <summary>
		/// Renders the post pipeline.
		/// </summary>
		/// <param name="n_args"> The number textures being sent to the pipeline. </param>
		/// <param name="textures"> A list of textures in indexed order to be bound for the shader program. </param>
		virtual void renderPipeline(const int n_args, ...) = 0;

		/// <summary>
		/// Gets the fbo containing the pipeline output.
		/// </summary>
		/// <returns> The output fbo. </returns>
		virtual fbo *getOutput() = 0;
	};
}
