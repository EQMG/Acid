#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT GreyFilter : public PostFilter {
public:
	explicit GreyFilter(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;
};
}
