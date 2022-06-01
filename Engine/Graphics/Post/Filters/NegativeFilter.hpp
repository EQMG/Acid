#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_GRAPHICS_EXPORT NegativeFilter : public PostFilter {
public:
	explicit NegativeFilter(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;
};
}
