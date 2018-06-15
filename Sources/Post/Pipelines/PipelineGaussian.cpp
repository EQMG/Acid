#include "PipelineGaussian.hpp"

namespace fl
{
	PipelineGaussian::PipelineGaussian(const GraphicsStage &graphicsStage, const float &scale) :
		IPostPipeline(graphicsStage),
		m_filterBlurVertical(FilterBlurVertical(graphicsStage, scale)),
		m_filterBlurHorizontal(FilterBlurHorizontal(graphicsStage, scale))
	{
	}

	PipelineGaussian::~PipelineGaussian()
	{
	}

	void PipelineGaussian::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_filterBlurVertical.Render(commandBuffer, clipPlane, camera);
		m_filterBlurHorizontal.Render(commandBuffer, clipPlane, camera);
	}
}
