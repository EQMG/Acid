#include "pipelinebloom.hpp"

namespace flounder
{
	pipelinebloom::pipelinebloom() :
		ipostpipeline()
	{
		m_filterBloom1 = new filterbloom1();
		m_pipelineGaussian = new pipelinegaussian(0.5f);
		m_filterBloom2 = new filterbloom2();
	}

	pipelinebloom::~pipelinebloom()
	{
		delete m_filterBloom1;
		delete m_pipelineGaussian;
		delete m_filterBloom2;
	}

	void pipelinebloom::renderPipeline(const int n_args, va_list args)
	{
		m_filterBloom1->applyFilter(n_args, args);
#if 0
		m_pipelineGaussian->renderPipelineV(1, m_filterBloom1->getFbo()->getColourTexture(0));
		m_filterBloom2->applyFilter(2, args[0], m_pipelineGaussian->getOutput()->getColourTexture(0)); // Blurred - Colour
#endif
	}

	Fbo *pipelinebloom::getOutput()
	{
		return m_filterBloom2->getFbo();
	}
}
