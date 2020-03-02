#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT GrainFilter : public PostFilter {
public:
	explicit GrainFilter(const Pipeline::Stage &pipelineStage, float strength = 2.3f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetStrength() const { return strength; }
	void SetStrength(float strength) { this->strength = strength; }

private:
	PushHandler pushScene;

	float strength;
};
}
