#pragma once

#include "Post/Filters/FilterBlur.hpp"
#include "Post/PostPipeline.hpp"

namespace acid {
class ACID_EXPORT PipelineBlur : public PostPipeline {
public:
	explicit PipelineBlur(const Pipeline::Stage &pipelineStage, float blur = 2.0f, const FilterBlur::Type & blurType = FilterBlur::Type::_9, bool toScreen = false,
		float inputScale = 0.5f, float outputScale = 1.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	float GetInputScale() const { return m_inputScale; }
	void SetInputScale(float inputScale) { m_inputScale = inputScale; }

	float GetOutputScale() const { return m_outputScale; }
	void SetOutputScale(float outputScale) { m_outputScale = outputScale; }

	float GetBlur() const { return m_blur; }
	void SetBlur(float blur) { m_blur = blur; }

	const Image2d *GetOutput() const { return m_output.get(); }

private:
	FilterBlur m_filterBlurVertical;
	FilterBlur m_filterBlurHorizontal;

	bool m_toScreen;
	float m_inputScale;
	float m_outputScale;
	float m_blur;

	std::unique_ptr<Image2d> m_output;
	Vector2i m_lastSize;
};
}
