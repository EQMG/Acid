#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class FilterNegative :
	public PostFilter
{
public:
	explicit FilterNegative(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;
};
}
