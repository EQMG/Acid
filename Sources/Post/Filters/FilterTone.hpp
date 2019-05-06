#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterTone :
	public PostFilter
{
public:
	explicit FilterTone(const Pipeline::Stage &pipelineStage);

	void Record(const CommandBuffer &commandBuffer) override;
};
}
