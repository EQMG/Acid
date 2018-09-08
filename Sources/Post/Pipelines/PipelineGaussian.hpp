#pragma once

#include "Post/Filters/FilterBlurHorizontal.hpp"
#include "Post/Filters/FilterBlurVertical.hpp"
#include "Post/IPostPipeline.hpp"

namespace acid
{
	class ACID_EXPORT PipelineGaussian :
		public IPostPipeline
	{
	private:
		FilterBlurVertical m_filterBlurVertical;
		FilterBlurHorizontal m_filterBlurHorizontal;
	public:
		PipelineGaussian(const GraphicsStage &graphicsStage, const float &scale = 2.0f);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
