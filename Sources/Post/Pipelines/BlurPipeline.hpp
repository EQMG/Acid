#pragma once

#include "Post/Filters/BlurFilter.hpp"
#include "Post/PostPipeline.hpp"

namespace acid {
class ACID_EXPORT BlurPipeline : public PostPipeline {
public:
	explicit BlurPipeline(const Pipeline::Stage &pipelineStage, float blur = 2.0f, const BlurFilter::Type &blurType = BlurFilter::Type::_9, bool toScreen = false,
		float inputScale = 0.5f, float outputScale = 1.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetInputScale() const { return inputScale; }
	void SetInputScale(float inputScale) { this->inputScale = inputScale; }

	float GetOutputScale() const { return outputScale; }
	void SetOutputScale(float outputScale) { this->outputScale = outputScale; }

	float GetBlur() const { return blur; }
	void SetBlur(float blur) { this->blur = blur; }

	const Image2d *GetOutput() const { return output.get(); }

private:
	BlurFilter filterBlurVertical, filterBlurHorizontal;

	bool toScreen;
	float inputScale;
	float outputScale;
	float blur;

	std::unique_ptr<Image2d> output;
	Vector2i lastSize;
};
}
