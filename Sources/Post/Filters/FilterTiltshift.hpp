#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterTiltshift : public PostFilter
{
  public:
	explicit FilterTiltshift(const Pipeline::Stage& pipelineStage, const float& blurAmount = 1.0f, const float& centre = 1.1f, const float& stepSize = 0.004f, const float& steps = 3.0f);

	void Render(const CommandBuffer& commandBuffer) override;

	const float& GetBlurAmount() const
	{
		return m_blurAmount;
	}

	void SetBlurAmount(const float& blurAmount)
	{
		m_blurAmount = blurAmount;
	}

	const float& GetCentre() const
	{
		return m_centre;
	}

	void SetCentre(const float& centre)
	{
		m_centre = centre;
	}

	const float& GetStepSize() const
	{
		return m_stepSize;
	}

	void SetStepSize(const float& stepSize)
	{
		m_stepSize = stepSize;
	}

	const float& GetSteps() const
	{
		return m_steps;
	}

	void SetSteps(const float& steps)
	{
		m_steps = steps;
	}

  private:
	PushHandler m_pushScene;

	float m_blurAmount;
	float m_centre;
	float m_stepSize;
	float m_steps;
};
}
