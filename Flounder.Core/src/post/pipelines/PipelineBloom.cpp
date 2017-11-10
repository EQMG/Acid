#include "PipelineBloom.hpp"

namespace Flounder
{
	PipelineBloom::PipelineBloom() :
		IPostPipeline()
	{
		m_filterBloom1 = new FilterBloom1();
		m_pipelineGaussian = new PipelineGaussian(0.5f);
		m_filterBloom2 = new FilterBloom2();
	}

	PipelineBloom::~PipelineBloom()
	{
		delete m_filterBloom1;
		delete m_pipelineGaussian;
		delete m_filterBloom2;
	}

	void PipelineBloom::RenderPipeline(const VkCommandBuffer *commandBuffer)
	{
		m_filterBloom1->RenderFilter(commandBuffer);
		m_pipelineGaussian->RenderPipeline(commandBuffer);
		m_filterBloom2->RenderFilter(commandBuffer);
	}
}
