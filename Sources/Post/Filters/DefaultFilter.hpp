#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT DefaultFilter : public PostFilter {
public:
	explicit DefaultFilter(const Pipeline::Stage &pipelineStage, bool lastFilter = false);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	bool lastFilter;
};
}
