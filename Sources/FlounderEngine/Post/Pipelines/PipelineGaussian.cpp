#include "PipelineGaussian.hpp"

namespace Flounder
{
	PipelineGaussian::PipelineGaussian(const int &subpass, const int &width, const int &height) :
		IPostPipeline(),
		m_filterBlurHorizontal(new FilterBlurHorizontal(subpass, width, height)),
		m_filterBlurVertical(new FilterBlurVertical(subpass, width, height))
	{
	}

	PipelineGaussian::PipelineGaussian(const int &subpass, const float &sizeScalar) :
		IPostPipeline(),
		m_filterBlurHorizontal(new FilterBlurHorizontal(subpass, sizeScalar)),
		m_filterBlurVertical(new FilterBlurVertical(subpass, sizeScalar))
	{
	}

	PipelineGaussian::~PipelineGaussian()
	{
		delete m_filterBlurHorizontal;
		delete m_filterBlurVertical;
	}

	void PipelineGaussian::RenderPipeline(const VkCommandBuffer &commandBuffer)
	{
		m_filterBlurHorizontal->RenderFilter(commandBuffer);
		m_filterBlurVertical->RenderFilter(commandBuffer);
	}

	void PipelineGaussian::SetScaleValue(const float &scaleValue) const
	{
		m_filterBlurHorizontal->SetBlurAmount(scaleValue);
		m_filterBlurVertical->SetBlurAmount(scaleValue);
	}
}
