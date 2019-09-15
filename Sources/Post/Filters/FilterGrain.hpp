#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT
FilterGrain
:
public
PostFilter
{
public:
	explicit FilterGrain(const Pipeline::Stage &pipelineStage, float strength = 2.3f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetStrength() const { return m_strength; }

	void SetStrength(float strength) { m_strength = strength; }

private:
	PushHandler m_pushScene;

	float m_strength;
};
}
