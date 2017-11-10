#include "PipelineGaussian.hpp"

namespace Flounder
{
	PipelineGaussian::PipelineGaussian(const int &width, const int &height) :
		IPostPipeline()
	{
		m_filterBlurHorizontal = new FilterBlurHorizontal(width, height);
		m_filterBlurVertical = new FilterBlurVertical(width, height);
	}

	PipelineGaussian::PipelineGaussian(const float &sizeScalar) :
		IPostPipeline()
	{
		m_filterBlurHorizontal = new FilterBlurHorizontal(sizeScalar);
		m_filterBlurVertical = new FilterBlurVertical(sizeScalar);
	}

	PipelineGaussian::~PipelineGaussian()
	{
		delete m_filterBlurHorizontal;
		delete m_filterBlurVertical;
	}

	void PipelineGaussian::RenderPipeline(const VkCommandBuffer *commandBuffer)
	{
		m_filterBlurHorizontal->RenderFilter(commandBuffer);
		m_filterBlurVertical->RenderFilter(commandBuffer);
	}

	void PipelineGaussian::SetScaleValue(const float &scaleValue) const
	{
		m_filterBlurHorizontal->SetScaleValue(scaleValue);
		m_filterBlurVertical->SetScaleValue(scaleValue);
	}
}
