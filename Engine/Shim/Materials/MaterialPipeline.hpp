#pragma once

#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Graphics/RenderStage.hpp"
#include "Shim/Export.hpp"

namespace acid {
/**
 * @brief Resource that represents a material pipeline.
 */
class ACID_SHIM_EXPORT MaterialPipeline : public Resource {
public:
	/**
	 * Creates a new material pipeline, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The material pipeline with the requested values.
	 */
	static std::shared_ptr<MaterialPipeline> Create(const Node &node);

	/**
	 * Creates a new material pipeline, or finds one with the same values.
	 * @param pipelineStage Stage the pipeline will be executed on.
	 * @param pipelineCreate Information used to define pipeline properties.
	 * @return The material pipeline with the requested values.
	 */
	static std::shared_ptr<MaterialPipeline> Create(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate);

	/**
	 * Creates a new material pipeline.
	 * @param pipelineStage Stage the pipeline will be executed on.
	 * @param pipelineCreate Information used to define pipeline properties.
	 */
	MaterialPipeline(Pipeline::Stage pipelineStage = {}, PipelineGraphicsCreate pipelineCreate = {});

	/**
	 * Binds this pipeline to the current renderpass.
	 * @param commandBuffer The command buffer to write to.
	 * @return If the pipeline has been bound successfully.
	 */
	bool BindPipeline(const CommandBuffer &commandBuffer);

	std::type_index GetTypeIndex() const override { return typeid(MaterialPipeline); }

	const Pipeline::Stage &GetStage() const { return pipelineStage; }
	const PipelineGraphicsCreate &GetPipelineCreate() const { return pipelineCreate; }
	const PipelineGraphics *GetPipeline() const { return pipeline.get(); }

	friend const Node &operator>>(const Node &node, MaterialPipeline &pipeline);
	friend Node &operator<<(Node &node, const MaterialPipeline &pipeline);

private:
	// TODO: Have pipelineStage configured though the application rather than hardcoded in the Material implementations.
	Pipeline::Stage pipelineStage;
	PipelineGraphicsCreate pipelineCreate;
	const RenderStage *renderStage = nullptr;
	std::unique_ptr<PipelineGraphics> pipeline;
};
}
