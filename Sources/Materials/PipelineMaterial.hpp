#pragma once

#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Renderer/RenderStage.hpp"

namespace acid
{
/**
 * @brief Resource that represents a material pipeline.
 */
class ACID_EXPORT PipelineMaterial :
	public Resource
{
public:
	/**
	 * Creates a new material pipeline, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The material pipeline with the requested values.
	 */
	static std::shared_ptr<PipelineMaterial> Create(const Metadata &metadata);

	/**
	 * Creates a new material pipeline, or finds one with the same values.
	 * @param pipelineStage Stage the pipeline will be executed on.
	 * @param pipelineCreate Information used to define pipeline properties.
	 * @return The material pipeline with the requested values.
	 */
	static std::shared_ptr<PipelineMaterial> Create(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate);

	/**
	 * Creates a new material pipeline.
	 * @param pipelineStage Stage the pipeline will be executed on.
	 * @param pipelineCreate Information used to define pipeline properties.
	 */
	PipelineMaterial(Pipeline::Stage pipelineStage, PipelineGraphicsCreate pipelineCreate);

	/**
	 * Binds this pipeline to the current renderpass.
	 * @param commandBuffer The command buffer to write to.
	 * @return If the pipeline has been bound successfully.
	 */
	bool BindPipeline(const CommandBuffer &commandBuffer);

	const Pipeline::Stage &GetStage() const { return m_pipelineStage; }

	const PipelineGraphicsCreate &GetPipelineCreate() const { return m_pipelineCreate; }

	const PipelineGraphics *GetPipeline() { return m_pipeline.get(); }

	ACID_EXPORT friend const Metadata &operator>>(const Metadata &metadata, PipelineMaterial &pipeline);

	ACID_EXPORT friend Metadata &operator<<(Metadata &metadata, const PipelineMaterial &pipeline);

private:
	Pipeline::Stage m_pipelineStage;
	PipelineGraphicsCreate m_pipelineCreate;
	const RenderStage *m_renderStage;
	std::unique_ptr<PipelineGraphics> m_pipeline;
};
}
