#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterWobble : public PostFilter
{
  public:
	explicit FilterWobble(const Pipeline::Stage& pipelineStage, const float& wobbleSpeed = 2.0f);

	void Render(const CommandBuffer& commandBuffer) override;

	const float& GetWobbleSpeed() const
	{
		return m_wobbleSpeed;
	}

	void SetWobbleSpeed(const float& wobbleSpeed)
	{
		m_wobbleSpeed = wobbleSpeed;
	}

  private:
	PushHandler m_pushScene;

	float m_wobbleSpeed;
	float m_wobbleAmount;
};
}
