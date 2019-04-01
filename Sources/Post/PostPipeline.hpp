#pragma once

#include "Renderer/RenderPipeline.hpp"

namespace acid
{
/**
 * @brief Represents a system of post effects.
 */
class ACID_EXPORT PostPipeline :
	public RenderPipeline
{
public:
	/**
	 * Creates a new post pipeline.
	 * @param pipelineStage The pipelines graphics stage.
	 */
	explicit PostPipeline(const Pipeline::Stage &pipelineStage) :
		RenderPipeline(pipelineStage)
	{
	}

	virtual ~PostPipeline() = default;
};
}
