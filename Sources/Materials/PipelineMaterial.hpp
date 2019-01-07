#pragma once

#include "Models/Model.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Renderer/RenderStage.hpp"
#include "Resources/Resource.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a material pipeline.
	/// </summary>
	class ACID_EXPORT PipelineMaterial :
		public Resource
	{
	private:
		GraphicsStage m_graphicsStage;
		PipelineCreate m_pipelineCreate;
		const RenderStage *m_renderStage;
		std::unique_ptr<PipelineGraphics> m_pipeline;
	public:
		/// <summary>
		/// Will find an existing pipeline with the same stage and create info, or create a new pipeline.
		/// </summary>
		/// <param name="graphicsStage"> Stage the pipeline will be executed on. </param>
		/// <param name="pipelineCreate"> Information used to define pipeline properties. </param>
		static std::shared_ptr<PipelineMaterial> Create(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);

		/// <summary>
		/// Creates a new material pipeline.
		/// </summary>
		/// <param name="graphicsStage"> Stage the pipeline will be executed on. </param>
		/// <param name="pipelineCreate"> Information used to define pipeline properties. </param>
		PipelineMaterial(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);

		/// <summary>
		/// Binds this pipeline to the current renderpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to write to. </param>
		bool BindPipeline(const CommandBuffer &commandBuffer);

		const GraphicsStage &GetGraphicsStage() const { return m_graphicsStage; }

		const PipelineCreate &GetPipelineCreate() const { return m_pipelineCreate; }

		const PipelineGraphics *GetPipeline() { return m_pipeline.get(); }
	private:
		static std::string ToName(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);
	};
}
