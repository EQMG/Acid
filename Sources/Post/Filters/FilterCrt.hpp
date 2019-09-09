#pragma once

#include "Maths/Colour.hpp"
#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterCrt :
	public PostFilter
{
public:
	explicit FilterCrt(const Pipeline::Stage &pipelineStage, const Colour &screenColour = Colour(0.5f, 1.0f, 0.5f), float curveAmountX = 0.1f,
		float curveAmountY = 0.1f, float scanLineSize = 1000.0f, float scanIntensity = 0.1f);

	void Render(const CommandBuffer &commandBuffer) override;

	const Colour &GetScreenColour() const { return m_screenColour; }

	void SetScreenColour(const Colour &screenColour) { m_screenColour = screenColour; }

	float GetCurveAmountX() const { return m_curveAmountX; }

	void SetCurveAmountX(float curveAmountX) { m_curveAmountX = curveAmountX; }

	float GetCurveAmountY() const { return m_curveAmountY; }

	void SetCurveAmountY(float curveAmountY) { m_curveAmountY = curveAmountY; }

	float GetScanLineSize() const { return m_scanLineSize; }

	void SetScanLineSize(float scanLineSize) { m_scanLineSize = scanLineSize; }

	float GetScanIntensity() const { return m_scanIntensity; }

	void SetScanIntensity(float scanIntensity) { m_scanIntensity = scanIntensity; }

private:
	PushHandler m_pushScene;

	Colour m_screenColour;
	float m_curveAmountX;
	float m_curveAmountY;
	float m_scanLineSize;
	float m_scanIntensity;
};
}
