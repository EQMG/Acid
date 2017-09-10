#pragma once

#include "../filters/FilterBlurHorizontal.hpp"
#include "../filters/FilterBlurVertical.hpp"
#include "../IPostPipeline.hpp"

namespace Flounder
{
	class PipelineGaussian :
		public IPostPipeline
	{
	private:
		filterblurhorizontal *m_filterBlurHorizontal;
		filterblurvertical *m_filterBlurVertical;
	public:
		PipelineGaussian(const int &width, const int &height);

		PipelineGaussian(const float &sizeScalar);

		~PipelineGaussian();

		void RenderPipeline(const int n_args, va_list args) override;

		Fbo *GetOutput() override;

		void SetScaleValue(const float &scaleValue) const;
	};
}
