#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT FilterFxaa : public PostFilter {
public:
	explicit FilterFxaa(const Pipeline::Stage &pipelineStage, float spanMax = 8.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetSpanMax() const { return spanMax; }
	void SetSpanMax(float spanMax) { this->spanMax = spanMax; }

private:
	PushHandler pushScene;

	float spanMax;
};
}
