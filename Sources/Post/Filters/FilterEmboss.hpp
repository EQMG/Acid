#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterEmboss : public PostFilter
{
  public:
	explicit FilterEmboss(const Pipeline::Stage& pipelineStage);

	void Render(const CommandBuffer& commandBuffer) override;
};
}
