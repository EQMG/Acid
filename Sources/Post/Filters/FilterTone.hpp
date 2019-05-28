#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class FilterTone :
	public PostFilter
{
public:
	explicit FilterTone(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;
};
}
