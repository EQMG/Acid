#pragma once

#include "Maths/Vector3.hpp"
#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterLensflare : public PostFilter
{
  public:
	explicit FilterLensflare(const Pipeline::Stage& pipelineStage);

	void Render(const CommandBuffer& commandBuffer) override;

	const Vector3& GetSunPosition() const
	{
		return m_sunPosition;
	}

	void SetSunPosition(const Vector3& sunPosition);

	const float& GetSunHeight() const
	{
		return m_sunHeight;
	}

	void SetSunHeight(const float& sunHeight)
	{
		m_sunHeight = sunHeight;
	}

  private:
	PushHandler m_pushScene;

	Vector3 m_sunPosition;
	float m_sunHeight;
};
}
