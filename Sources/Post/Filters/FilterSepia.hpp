#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterSepia : public PostFilter
{
  public:
	explicit FilterSepia(const Pipeline::Stage& pipelineStage);

	void Render(const CommandBuffer& commandBuffer) override;
};
}
