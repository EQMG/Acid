#pragma once

#include <Post/PostFilter.hpp>

using namespace acid;

namespace acid {
class FilterBlit : public PostFilter {
public:
	explicit FilterBlit(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	bool m_lastFilter;
};
}
