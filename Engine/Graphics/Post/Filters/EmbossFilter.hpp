#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_GRAPHICS_EXPORT EmbossFilter : public PostFilter {
public:
	explicit EmbossFilter(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;
};
}
