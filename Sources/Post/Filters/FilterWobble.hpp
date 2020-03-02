#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT FilterWobble : public PostFilter {
public:
	explicit FilterWobble(const Pipeline::Stage &pipelineStage, float wobbleSpeed = 2.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetWobbleSpeed() const { return wobbleSpeed; }
	void SetWobbleSpeed(float wobbleSpeed) { this->wobbleSpeed = wobbleSpeed; }

private:
	PushHandler pushScene;

	float wobbleSpeed;
	float wobbleAmount;
};
}
