#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT
FilterPixel
:
public
PostFilter
{
public:
	explicit FilterPixel(const Pipeline::Stage &pipelineStage, float pixelSize = 2.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetPixelSize() const { return m_pixelSize; }

	void SetPixelSize(float pixelSize) { m_pixelSize = pixelSize; }

private:
	PushHandler m_pushScene;

	float m_pixelSize;
};
}
