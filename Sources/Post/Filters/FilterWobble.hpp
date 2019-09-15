#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT
FilterWobble
:
public
PostFilter
{
public:
	explicit FilterWobble(const Pipeline::Stage &pipelineStage, float wobbleSpeed = 2.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetWobbleSpeed() const { return m_wobbleSpeed; }

	void SetWobbleSpeed(float wobbleSpeed) { m_wobbleSpeed = wobbleSpeed; }

private:
	PushHandler m_pushScene;

	float m_wobbleSpeed;
	float m_wobbleAmount;
};
}
