#pragma once

#include "Models/Model.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Resources/IResource.hpp"
#include "Textures/Texture.hpp"

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
		Pipeline m_pipeline;
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

		~PipelineMaterial();

		std::string GetFilename() override { return m_filename; }

		Pipeline &GetPipeline() { return m_pipeline; }

	private:
		static std::string ToFilename(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate);
	};
}
