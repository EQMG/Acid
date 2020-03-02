#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT TiltshiftFilter : public PostFilter {
public:
	explicit TiltshiftFilter(const Pipeline::Stage &pipelineStage, float blurAmount = 1.0f, float centre = 1.1f, float stepSize = 0.004f,
		float steps = 3.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetBlurAmount() const { return blurAmount; }
	void SetBlurAmount(float blurAmount) { this->blurAmount = blurAmount; }

	float GetCentre() const { return centre; }
	void SetCentre(float centre) { this->centre = centre; }

	float GetStepSize() const { return stepSize; }
	void SetStepSize(float stepSize) { this->stepSize = stepSize; }

	float GetSteps() const { return steps; }
	void SetSteps(float steps) { this->steps = steps; }

private:
	PushHandler pushScene;

	float blurAmount;
	float centre;
	float stepSize;
	float steps;
};
}
