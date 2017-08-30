#include "pipelinegaussian.hpp"

namespace flounder
{
	pipelinegaussian::pipelinegaussian(const int &width, const int &height) :
		ipostpipeline()
	{
		m_filterBlurHorizontal = new filterblurhorizontal(width, height);
		m_filterBlurVertical = new filterblurvertical(width, height);
	}

	pipelinegaussian::pipelinegaussian(const float &sizeScalar) :
		ipostpipeline()
	{
		m_filterBlurHorizontal = new filterblurhorizontal(sizeScalar);
		m_filterBlurVertical = new filterblurvertical(sizeScalar);
	}

	pipelinegaussian::~pipelinegaussian()
	{
		delete m_filterBlurHorizontal;
		delete m_filterBlurVertical;
	}

	void pipelinegaussian::renderPipeline(const int n_args, va_list args)
	{
#if 0
		m_filterBlurHorizontal->applyFilter(1, args[0]);
		m_filterBlurVertical->applyFilter(1, m_filterBlurHorizontal->getFbo()->getColourTexture(0));
#endif
	}

	Fbo *pipelinegaussian::getOutput()
	{
		return m_filterBlurVertical->getFbo();
	}

	void pipelinegaussian::setScaleValue(const float &scaleValue)
	{
		m_filterBlurHorizontal->setScaleValue(scaleValue);
		m_filterBlurVertical->setScaleValue(scaleValue);
	}
}
