#pragma once

#include "Models/Model.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Renderer/RenderStage.hpp"
#include "Resources/IResource.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a material pipeline.
	/// </summary>
	class ACID_EXPORT PipelineMaterial :
		public IResource
	{
	private:
		std::string m_filename;
		GraphicsStage m_graphicsStage;
		PipelineCreate m_pipelineCreate;
		RenderStage *m_renderStage;
		std::unique_ptr<Pipeline> m_pipeline;
	public:
		/// <summary>
		/// Will find an existing pipeline with the same stage and create info, or create a new pipeline.
		/// </summary>
		/// <param name="graphicsStage"> Stage the pipeline will be executed on. </param>
		/// <param name="pipelineCreate"> Information used to define pipeline properties. </param>
		static std::shared_ptr<PipelineMaterial> Resource(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);

		/// <summary>
		/// Creates a new material pipeline.
		/// </summary>
		/// <param name="graphicsStage"> Stage the pipeline will be executed on. </param>
		/// <param name="pipelineCreate"> Information used to define pipeline properties. </param>
		PipelineMaterial(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);

		bool BindPipeline(const CommandBuffer &commandBuffer);

		std::string GetFilename() override { return m_filename; }

		GraphicsStage GetGraphicsStage() const { return m_graphicsStage; }

		PipelineCreate GetPipelineCreate() const { return m_pipelineCreate; }

		Pipeline *GetPipeline() { return m_pipeline.get(); }
	private:
		static std::string ToFilename(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);
	};
}
