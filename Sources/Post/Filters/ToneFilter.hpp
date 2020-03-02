#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT ToneFilter : public PostFilter {
public:
	explicit ToneFilter(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;
};
}
