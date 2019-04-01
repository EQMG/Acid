#pragma once

#include "Renderer/RenderPipeline.hpp"

namespace acid
{
/**
 * Represents a system of post effects.
 */
class ACID_EXPORT PostPipeline :
	public RenderPipeline
{
public:
	/**
	 * Creates a new post pipeline.
	 * @param pipelineStageThe pipelines graphics stage.
	 */
	explicit PostPipeline(const Pipeline::Stage &pipelineStage) :
		RenderPipeline(pipelineStage)
	{
	}

	virtual ~PostPipeline() = default;
};
}
