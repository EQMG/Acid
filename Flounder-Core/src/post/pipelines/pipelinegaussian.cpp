#include "pipelinegaussian.h"

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

	void pipelinegaussian::renderPipeline(const int n_args, ...)
	{
	//	m_filterBlurHorizontal->applyFilter(textures);
	//	m_filterBlurVertical->applyFilter(m_filterBlurHorizontal->getFbo()->getColourTexture(0));
	}

	fbo *pipelinegaussian::getOutput()
	{
		return m_filterBlurVertical->getFbo();
	}

	void pipelinegaussian::setScaleValue(const float &scaleValue)
	{
		m_filterBlurHorizontal->setScaleValue(scaleValue);
		m_filterBlurVertical->setScaleValue(scaleValue);
	}
}
