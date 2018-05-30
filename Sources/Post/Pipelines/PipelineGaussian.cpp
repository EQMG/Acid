#include "PipelineGaussian.hpp"

namespace fl
{
	PipelineGaussian::PipelineGaussian(const GraphicsStage &graphicsStage, const float &scale) :
		m_filterBlurVertical(FilterBlurVertical(graphicsStage, scale)),
		m_filterBlurHorizontal(FilterBlurHorizontal(graphicsStage, scale))
	{
	}

	PipelineGaussian::~PipelineGaussian()
	{
	}

	void PipelineGaussian::Render(const CommandBuffer &commandBuffer)
	{
		m_filterBlurVertical.Render(commandBuffer);
		m_filterBlurHorizontal.Render(commandBuffer);
	}
}
