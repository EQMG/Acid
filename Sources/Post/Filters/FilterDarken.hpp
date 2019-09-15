#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT
FilterDarken
:
public
PostFilter
{
public:
	explicit FilterDarken(const Pipeline::Stage &pipelineStage, float factor = 0.5f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetFactor() const { return m_factor; }

	void SetFactor(float factor) { m_factor = factor; }

private:
	PushHandler m_pushScene;

	float m_factor;
};
}
