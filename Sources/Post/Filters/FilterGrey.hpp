#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterGrey : public PostFilter
{
  public:
	explicit FilterGrey(const Pipeline::Stage& pipelineStage);

	void Render(const CommandBuffer& commandBuffer) override;
};
}
