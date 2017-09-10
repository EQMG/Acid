#pragma once

#include "../filters/FilterBloom1.hpp"
#include "../filters/FilterBloom2.hpp"
#include "../IPostPipeline.hpp"
#include "PipelineGaussian.hpp"

namespace Flounder
{
	class PipelineBloom :
		public IPostPipeline
	{
	private:
		filterbloom1 *m_filterBloom1;
		PipelineGaussian *m_pipelineGaussian;
		filterbloom2 *m_filterBloom2;
	public:
		PipelineBloom();

		~PipelineBloom();

		void RenderPipeline(const int n_args, va_list args) override;

		Fbo *GetOutput() override;
	};
}
