#pragma once

#include "../filters/filtercombine.h"
#include "../filters/filterdarken.h"
#include "../ipostpipeline.h"
#include "pipelinegaussian.h"

namespace flounder
{
	class pipelinepaused :
		public ipostpipeline
	{
	private:
		filterdarken *m_filterDarken;
		pipelinegaussian *m_pipelineGaussian1;
		pipelinegaussian *m_pipelineGaussian2;
		filtercombine *m_filterCombine;
		float m_blurFactor;
	public:
		pipelinepaused();

		~pipelinepaused();

		void renderPipeline(const int n_args, ...) override;

		fbo *getOutput() override;

		inline void setBlurFactor(const float &blurFactor) { m_blurFactor = blurFactor; }
	};
}
