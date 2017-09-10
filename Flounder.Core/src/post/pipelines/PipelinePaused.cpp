#include "PipelinePaused.hpp"

namespace Flounder
{
	PipelinePaused::PipelinePaused() :
		IPostPipeline()
	{
		m_filterDarken = new FilterDarken();
		m_pipelineGaussian1 = new PipelineGaussian(1.0f / 10.0f);
		m_pipelineGaussian2 = new PipelineGaussian(1.0f / 7.0f);
		m_filterCombine = new FilterCombine();

		m_blurFactor = 0.0f;
	}

	PipelinePaused::~PipelinePaused()
	{
		delete m_filterDarken;
		delete m_pipelineGaussian1;
		delete m_pipelineGaussian2;
		delete m_filterCombine;
	}

	void PipelinePaused::RenderPipeline(const int n_args, va_list args)
	{
#if 0
		m_pipelineGaussian1->renderPipeline(n_args, args);

		m_pipelineGaussian2->setScaleValue(1.25f);
		m_pipelineGaussian2->renderPipelineV(1, m_pipelineGaussian1->getOutput()->getColourTexture(0));

		m_filterDarken->setFactorValue(Maths::Max(fabs(1.0f - m_blurFactor), 0.45f));
		m_filterDarken->applyFilter(1, m_pipelineGaussian2->getOutput()->getColourTexture(0));

		m_filterCombine->setSlideSpace(m_blurFactor, 1.0f, 0.0f, 1.0f);
		m_filterCombine->applyFilter(2, args[0], m_filterDarken->getFbo()->getColourTexture(0)); // Darken - Colour
#endif
	}

	Fbo *PipelinePaused::GetOutput()
	{
		return m_filterCombine->GetFbo();
	}
}
