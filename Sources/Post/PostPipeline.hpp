#pragma once

#include "Renderer/Render.hpp"

namespace acid
{
/**
 * @brief Represents a system of post effects.
 */
class ACID_EXPORT PostPipeline :
	public Render
{
public:
	/**
	 * Creates a new post pipeline.
	 * @param pipelineStage The pipelines graphics stage.
	 */
	explicit PostPipeline(const Pipeline::Stage &pipelineStage) :
		Render(pipelineStage)
	{
	}

	virtual ~PostPipeline() = default;
};
}
