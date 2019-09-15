#pragma once

#include "Graphics/Subrender.hpp"

namespace acid {
/**
 * @brief Represents a system of post effects.
 */
class ACID_EXPORT PostPipeline : public Subrender {
public:
	/**
	 * Creates a new post pipeline.
	 * @param pipelineStage The pipelines graphics stage.
	 */
	explicit PostPipeline(const Pipeline::Stage &pipelineStage) :
		Subrender(pipelineStage) {
	}

	virtual ~PostPipeline() = default;
};
}
