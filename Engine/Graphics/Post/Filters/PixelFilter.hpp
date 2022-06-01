#pragma once

#include "Post/PostFilter.hpp"

namespace acid {
class ACID_GRAPHICS_EXPORT PixelFilter : public PostFilter {
public:
	explicit PixelFilter(const Pipeline::Stage &pipelineStage, float pixelSize = 2.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetPixelSize() const { return pixelSize; }
	void SetPixelSize(float pixelSize) { this->pixelSize = pixelSize; }

private:
	PushHandler pushScene;

	float pixelSize;
};
}
