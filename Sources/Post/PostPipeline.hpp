#pragma once

#include "Renderer/RenderPipeline.hpp"
#include "PostFilter.hpp"

namespace acid
{
	/// <summary>
	/// Represents a system of post effects.
	/// </summary>
	class ACID_EXPORT PostPipeline :
		public RenderPipeline
	{
	public:
		/// <summary>
		/// Creates a new post pipeline.
		/// </summary>
		/// <param name="graphicsStage"> The pipelines graphics stage. </param>
		explicit PostPipeline(const GraphicsStage &graphicsStage) :
			RenderPipeline(graphicsStage)
		{
		}
	};
}
