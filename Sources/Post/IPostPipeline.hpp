#pragma once

#include "Post/IPostFilter.hpp"
#include "Renderer/IRenderer.hpp"

namespace fl
{
	/// <summary>
	/// Represents a system of post effects.
	/// </summary>
	class FL_EXPORT IPostPipeline :
		public IRenderer
	{
	public:
		/// <summary>
		/// Creates a new post pipeline.
		/// </summary>
		/// <param name="graphicsStage"> The pipelines graphics stage. </param>
		IPostPipeline(const GraphicsStage &graphicsStage);

		/// <summary>
		/// Deconstructor for the post pipeline.
		/// </summary>
		virtual ~IPostPipeline();

		/// <summary>
		/// Renders the post pipeline.
		/// </summary>
		virtual void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override = 0;
	};
}
