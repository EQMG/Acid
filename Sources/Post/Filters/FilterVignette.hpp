#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterVignette : public PostFilter
{
  public:
	explicit FilterVignette(const Pipeline::Stage& pipelineStage, const float& innerRadius = 0.15f, const float& outerRadius = 1.35f, const float& opacity = 0.85f);

	void Render(const CommandBuffer& commandBuffer) override;

	const float& GetInnerRadius() const
	{
		return m_innerRadius;
	}

	void SetInnerRadius(const float& innerRadius)
	{
		m_innerRadius = innerRadius;
	}

	const float& GetOuterRadius() const
	{
		return m_outerRadius;
	}

	void SetOuterRadius(const float& outerRadius)
	{
		m_outerRadius = outerRadius;
	}

	const float& GetOpacity() const
	{
		return m_opacity;
	}

	void SetOpacity(const float& opacity)
	{
		m_opacity = opacity;
	}

  private:
	PushHandler m_pushScene;

	float m_innerRadius;
	float m_outerRadius;
	float m_opacity;
};
}
