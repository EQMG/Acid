#include "PipelineBloom.hpp"

namespace Flounder
{
	PipelineBloom::PipelineBloom() :
		IPostPipeline()
	{
		m_filterBloom1 = new filterbloom1();
		m_pipelineGaussian = new PipelineGaussian(0.5f);
		m_filterBloom2 = new filterbloom2();
	}

	PipelineBloom::~PipelineBloom()
	{
		delete m_filterBloom1;
		delete m_pipelineGaussian;
		delete m_filterBloom2;
	}

	void PipelineBloom::RenderPipeline(const int n_args, va_list args)
	{
		m_filterBloom1->ApplyFilter(n_args, args);
#if 0
		m_pipelineGaussian->RenderPipeline(1, m_filterBloom1->GetFbo()->GetColourTexture(0));
		m_filterBloom2->ApplyFilter(2, args[0], m_pipelineGaussian->GetOutput()->GetColourTexture(0)); // Blurred - Colour
#endif
	}

	Fbo *PipelineBloom::GetOutput()
	{
		return m_filterBloom2->GetFbo();
	}
}
