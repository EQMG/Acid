#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	/// <summary>
	/// Represents a system of post effects.
	/// </summary>
	class FL_EXPORT IPostPipeline
	{
	public:
		/// <summary>
		/// Creates a new post pipeline.
		/// </summary>
		IPostPipeline();

		/// <summary>
		/// Deconstructor for the post pipeline.
		/// </summary>
		virtual ~IPostPipeline();

		/// <summary>
		/// Renders the post pipeline.
		/// </summary>
		virtual void RenderPipeline(const CommandBuffer &commandBuffer);
	};
}
