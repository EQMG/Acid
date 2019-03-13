#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterDefault : public PostFilter
{
  public:
	explicit FilterDefault(const Pipeline::Stage& pipelineStage, const bool& lastFilter = false);

	void Render(const CommandBuffer& commandBuffer) override;

  private:
	bool m_lastFilter;
};
}
