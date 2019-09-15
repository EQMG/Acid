#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT FilterTiltshift : public PostFilter {
public:
	explicit FilterTiltshift(const Pipeline::Stage &pipelineStage, float blurAmount = 1.0f, float centre = 1.1f, float stepSize = 0.004f,
		float steps = 3.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetBlurAmount() const { return m_blurAmount; }
	void SetBlurAmount(float blurAmount) { m_blurAmount = blurAmount; }

	float GetCentre() const { return m_centre; }
	void SetCentre(float centre) { m_centre = centre; }

	float GetStepSize() const { return m_stepSize; }
	void SetStepSize(float stepSize) { m_stepSize = stepSize; }

	float GetSteps() const { return m_steps; }
	void SetSteps(float steps) { m_steps = steps; }

private:
	PushHandler m_pushScene;

	float m_blurAmount;
	float m_centre;
	float m_stepSize;
	float m_steps;
};
}
