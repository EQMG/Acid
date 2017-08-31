#pragma once

#include "../filters/filtercombine.hpp"
#include "../filters/filterdarken.hpp"
#include "../ipostpipeline.hpp"
#include "pipelinegaussian.hpp"

namespace Flounder
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

		void renderPipeline(const int n_args, va_list args) override;

		Fbo *getOutput() override;

		inline void setBlurFactor(const float &blurFactor) { m_blurFactor = blurFactor; }
	};
}
