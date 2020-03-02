#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT FilterVignette : public PostFilter {
public:
	explicit FilterVignette(const Pipeline::Stage &pipelineStage, float innerRadius = 0.15f, float outerRadius = 1.35f, float opacity = 0.85f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetInnerRadius() const { return innerRadius; }
	void SetInnerRadius(float innerRadius) { this->innerRadius = innerRadius; }

	float GetOuterRadius() const { return outerRadius; }
	void SetOuterRadius(float outerRadius) { this->outerRadius = outerRadius; }

	float GetOpacity() const { return opacity; }
	void SetOpacity(float opacity) { this->opacity = opacity; }

private:
	PushHandler pushScene;

	float innerRadius, outerRadius;
	float opacity;
};
}
