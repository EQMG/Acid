#include "PipelinePaused.hpp"

#include "../../maths/Maths.hpp"

namespace Flounder
{
	PipelinePaused::PipelinePaused(const int &subpass) :
		IPostPipeline()
	{
		m_filterDarken = new FilterDarken(subpass);
		m_pipelineGaussian1 = new PipelineGaussian(subpass, 1.0f / 10.0f);
		m_pipelineGaussian2 = new PipelineGaussian(subpass, 1.0f / 7.0f);
		m_filterCombine = new FilterCombine(subpass);

		m_blurFactor = 0.0f;
	}

	PipelinePaused::~PipelinePaused()
	{
		delete m_filterDarken;
		delete m_pipelineGaussian1;
		delete m_pipelineGaussian2;
		delete m_filterCombine;
	}

	void PipelinePaused::RenderPipeline(const VkCommandBuffer *commandBuffer)
	{
		m_pipelineGaussian1->RenderPipeline(commandBuffer);

		m_pipelineGaussian2->SetScaleValue(1.25f);
		m_pipelineGaussian2->RenderPipeline(commandBuffer);

		m_filterDarken->SetFactorValue(Maths::Max(static_cast<float>(fabs(1.0f - m_blurFactor)), 0.45f));
		m_filterDarken->RenderFilter(commandBuffer);

		m_filterCombine->SetSlideSpace(m_blurFactor, 1.0f, 0.0f, 1.0f);
		m_filterCombine->RenderFilter(commandBuffer); // Darken - Colour
	}
}
