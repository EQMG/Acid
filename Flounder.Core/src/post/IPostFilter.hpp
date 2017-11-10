#pragma once

#include "../models/Model.hpp"
#include "../renderer/Renderer.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a post effect shader and on application saves the result into a fbo.
	/// </summary>
	class IPostFilter
	{
	private:
		static const DescriptorType typeUboScene;
		static const DescriptorType typeSamplerColour;
		static const PipelineCreateInfo pipelineCreateInfo;
	protected:
		Pipeline *m_pipeline;
		Model *m_model;
	public:
		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="filterName"> The name for the filter. </param>
		/// <param name="fragmentShader"> The fragment shader file. </param>
		IPostFilter(const std::string &filterName, const std::string &fragmentShader);

		/// <summary>
		/// Deconstructor for the post filter.
		/// </summary>
		virtual ~IPostFilter();

		/// <summary>
		/// Renders the filter.
		/// </summary>
		virtual void RenderFilter(const VkCommandBuffer *commandBuffer);
	};
}
