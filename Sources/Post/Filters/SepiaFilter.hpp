#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT SepiaFilter : public PostFilter {
public:
	explicit SepiaFilter(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;
};
}
