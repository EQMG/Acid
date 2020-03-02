#pragma once

#include "Maths/Colour.hpp"
#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT CrtFilter : public PostFilter {
public:
	explicit CrtFilter(const Pipeline::Stage &pipelineStage, const Colour &screenColour = Colour(0.5f, 1.0f, 0.5f), float curveAmountX = 0.1f,
		float curveAmountY = 0.1f, float scanLineSize = 1000.0f, float scanIntensity = 0.1f);

	void Render(const CommandBuffer &commandBuffer) override;

	const Colour &GetScreenColour() const { return screenColour; }
	void SetScreenColour(const Colour &screenColour) { this->screenColour = screenColour; }

	float GetCurveAmountX() const { return curveAmountX; }
	void SetCurveAmountX(float curveAmountX) { this->curveAmountX = curveAmountX; }

	float GetCurveAmountY() const { return curveAmountY; }
	void SetCurveAmountY(float curveAmountY) { this->curveAmountY = curveAmountY; }

	float GetScanLineSize() const { return scanLineSize; }
	void SetScanLineSize(float scanLineSize) { this->scanLineSize = scanLineSize; }

	float GetScanIntensity() const { return scanIntensity; }
	void SetScanIntensity(float scanIntensity) { this->scanIntensity = scanIntensity; }

private:
	PushHandler pushScene;

	Colour screenColour;
	float curveAmountX, curveAmountY;
	float scanLineSize;
	float scanIntensity;
};
}
