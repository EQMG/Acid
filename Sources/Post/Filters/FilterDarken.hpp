#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterDarken : public PostFilter
{
  public:
	explicit FilterDarken(const Pipeline::Stage& pipelineStage, const float& factor = 0.5f);

	void Render(const CommandBuffer& commandBuffer) override;

	const float& GetFactor() const
	{
		return m_factor;
	}

	void SetFactor(const float& factor)
	{
		m_factor = factor;
	}

  private:
	PushHandler m_pushScene;

	float m_factor;
};
}
