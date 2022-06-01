#pragma once

#include <Post/PostFilter.hpp>

namespace acid {
class ACID_GRAPHICS_EXPORT BlitFilter : public PostFilter {
public:
	explicit BlitFilter(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;
};
}
