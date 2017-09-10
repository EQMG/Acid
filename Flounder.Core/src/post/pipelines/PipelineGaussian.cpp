#include "PipelineGaussian.hpp"

namespace Flounder
{
	PipelineGaussian::PipelineGaussian(const int &width, const int &height) :
		IPostPipeline()
	{
		m_filterBlurHorizontal = new filterblurhorizontal(width, height);
		m_filterBlurVertical = new filterblurvertical(width, height);
	}

	PipelineGaussian::PipelineGaussian(const float &sizeScalar) :
		IPostPipeline()
	{
		m_filterBlurHorizontal = new filterblurhorizontal(sizeScalar);
		m_filterBlurVertical = new filterblurvertical(sizeScalar);
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
		m_filterBlurHorizontal->setScaleValue(scaleValue);
		m_filterBlurVertical->setScaleValue(scaleValue);
	}
}
