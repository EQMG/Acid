#pragma once

#include "Renderer/IRenderer.hpp"
#include "IPostFilter.hpp"

namespace acid
{
	/// <summary>
	/// Represents a system of post effects.
	/// </summary>
	class ACID_EXPORT IPostPipeline :
		public IRenderer
	{
	public:
		/// <summary>
		/// Creates a new post pipeline.
		/// </summary>
		/// <param name="graphicsStage"> The pipelines graphics stage. </param>
		explicit IPostPipeline(const GraphicsStage &graphicsStage) :
			IRenderer(graphicsStage)
		{
		}
	};
}
