#include "PipelineGaussian.hpp"

namespace fl
{
	PipelineGaussian::PipelineGaussian(const GraphicsStage &graphicsStage, const float &scale) :
		m_filterBlurVertical(new FilterBlurVertical(graphicsStage, scale)),
		m_filterBlurHorizontal(new FilterBlurHorizontal(graphicsStage, scale))
	{
	}

	PipelineGaussian::~PipelineGaussian()
	{
		delete m_filterBlurVertical;
		delete m_filterBlurHorizontal;
	}

	void PipelineGaussian::Render(const CommandBuffer &commandBuffer)
	{
		m_filterBlurVertical->Render(commandBuffer);
		m_filterBlurHorizontal->Render(commandBuffer);
	}
}
