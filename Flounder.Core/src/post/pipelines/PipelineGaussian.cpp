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

	void PipelineGaussian::RenderPipeline(const int n_args, va_list args)
	{
#if 0
		m_filterBlurHorizontal->ApplyFilter(1, args[0]);
		m_filterBlurVertical->ApplyFilter(1, m_filterBlurHorizontal->GetFbo()->GetColourTexture(0));
#endif
	}

	Fbo *PipelineGaussian::GetOutput()
	{
		return m_filterBlurVertical->GetFbo();
	}

	void PipelineGaussian::SetScaleValue(const float &scaleValue) const
	{
		m_filterBlurHorizontal->SetScaleValue(scaleValue);
		m_filterBlurVertical->SetScaleValue(scaleValue);
	}
}
