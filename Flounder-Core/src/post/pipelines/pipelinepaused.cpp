#include "pipelinepaused.h"

namespace flounder
{
	pipelinepaused::pipelinepaused() :
		ipostpipeline()
	{
		m_filterDarken = new filterdarken();
		m_pipelineGaussian1 = new pipelinegaussian(1.0f / 10.0f);
		m_pipelineGaussian2 = new pipelinegaussian(1.0f / 7.0f);
		m_filterCombine = new filtercombine();

		m_blurFactor = 0.0f;
	}

	pipelinepaused::~pipelinepaused()
	{
		delete m_filterDarken;
		delete m_pipelineGaussian1;
		delete m_pipelineGaussian2;
		delete m_filterCombine;
	}

	void pipelinepaused::renderPipeline(const int n_args, va_list args)
	{
		m_pipelineGaussian1->renderPipeline(n_args, args);

		m_pipelineGaussian2->setScaleValue(1.25f);
		m_pipelineGaussian2->renderPipelineV(1, m_pipelineGaussian1->getOutput()->getColourTexture(0));

		m_filterDarken->setFactorValue(__max(fabs(1.0f - m_blurFactor), 0.45f));
		m_filterDarken->applyFilter(1, m_pipelineGaussian2->getOutput()->getColourTexture(0));

		m_filterCombine->setSlideSpace(m_blurFactor, 1.0f, 0.0f, 1.0f);
		m_filterCombine->applyFilter(2, args[0], m_filterDarken->getFbo()->getColourTexture(0)); // Darken - Colour
	}

	fbo *pipelinepaused::getOutput()
	{
		return m_filterCombine->getFbo();
	}
}
