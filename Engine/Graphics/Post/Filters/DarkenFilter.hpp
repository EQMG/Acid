#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_GRAPHICS_EXPORT DarkenFilter : public PostFilter {
public:
	explicit DarkenFilter(const Pipeline::Stage &pipelineStage, float factor = 0.5f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetFactor() const { return factor; }
	void SetFactor(float factor) { this->factor = factor; }

private:
	PushHandler pushScene;

	float factor;
};
}
