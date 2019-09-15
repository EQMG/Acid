#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT
FilterVignette
:
public
PostFilter
{
public:
	explicit FilterVignette(const Pipeline::Stage &pipelineStage, float innerRadius = 0.15f, float outerRadius = 1.35f, float opacity = 0.85f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetInnerRadius() const { return m_innerRadius; }

	void SetInnerRadius(float innerRadius) { m_innerRadius = innerRadius; }

	float GetOuterRadius() const { return m_outerRadius; }

	void SetOuterRadius(float outerRadius) { m_outerRadius = outerRadius; }

	float GetOpacity() const { return m_opacity; }

	void SetOpacity(float opacity) { m_opacity = opacity; }

private:
	PushHandler m_pushScene;

	float m_innerRadius;
	float m_outerRadius;
	float m_opacity;
};
}
