#pragma once

#include "../filters/FilterCombine.hpp"
#include "../filters/FilterDarken.hpp"
#include "../IPostPipeline.hpp"
#include "PipelineGaussian.hpp"

namespace Flounder
{
	class PipelinePaused :
		public IPostPipeline
	{
	private:
		FilterDarken *m_filterDarken;
		PipelineGaussian *m_pipelineGaussian1;
		PipelineGaussian *m_pipelineGaussian2;
		FilterCombine *m_filterCombine;
		float m_blurFactor;
	public:
		PipelinePaused();

		~PipelinePaused();

		void RenderPipeline(const int n_args, va_list args) override;

		Fbo *GetOutput() override;

		void SetBlurFactor(const float &blurFactor) { m_blurFactor = blurFactor; }
	};
}
