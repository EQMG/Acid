#pragma once

#include "IPostFilter.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a system of post effects.
	/// </summary>
	class IPostPipeline
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
		virtual void RenderPipeline(const VkCommandBuffer &commandBuffer);
	};
}
