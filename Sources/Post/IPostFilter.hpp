#pragma once

#include "Models/Model.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace fl
{
	/// <summary>
	/// Represents a post effect shader and on application saves the result into a fbo.
	/// </summary>
	class FL_EXPORT IPostFilter
	{
	protected:
		DescriptorsHandler *m_descriptorSet;

		Pipeline *m_pipeline;
		Model *m_model;
	public:
		/// <summary>
		/// Creates a new post effect filter
		/// </summary>
		/// <param name="shaderStages"> The pipelines shader stages. </param>
		/// <param name="graphicsStage"> The pipelines graphics stage. </param>
		/// <param name="defines"> A list of names that will be added as a #define. </param>
		IPostFilter(const std::vector<std::string> &shaderStages, const GraphicsStage &graphicsStage, const std::vector<PipelineDefine> &defines = {});

		/// <summary>
		/// Deconstructor for the post filter.
		/// </summary>
		virtual ~IPostFilter();

		/// <summary>
		/// Renders the filter.
		/// </summary>
		virtual void Render(const CommandBuffer &commandBuffer) = 0;

		DescriptorsHandler *GetDescriptorSet() const { return m_descriptorSet; }

		Pipeline *GetPipeline() const { return m_pipeline; }

		Model *GetModel() const { return m_model; }
	};
}
