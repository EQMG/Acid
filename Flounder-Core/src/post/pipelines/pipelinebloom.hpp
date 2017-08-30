#pragma once

#include "../filters/filterbloom1.hpp"
#include "../filters/filterbloom2.hpp"
#include "../ipostpipeline.hpp"
#include "pipelinegaussian.hpp"

namespace flounder
{
	class pipelinebloom :
		public ipostpipeline
	{
	private:
		filterbloom1 *m_filterBloom1;
		pipelinegaussian *m_pipelineGaussian;
		filterbloom2 *m_filterBloom2;
	public:
		pipelinebloom();

		~pipelinebloom();

		void renderPipeline(const int n_args, va_list args) override;

		Fbo *getOutput() override;
	};
}
