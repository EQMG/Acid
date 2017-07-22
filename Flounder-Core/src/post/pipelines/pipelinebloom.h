#pragma once

#include "../filters/filterbloom1.h"
#include "../filters/filterbloom2.h"
#include "../ipostpipeline.h"
#include "pipelinegaussian.h"

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

		void renderPipeline(const int n_args, ...) override;

		fbo *getOutput() override;
	};
}
