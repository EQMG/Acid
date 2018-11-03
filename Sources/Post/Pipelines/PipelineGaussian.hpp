#pragma once

#include "Post/Filters/FilterBlur.hpp"
#include "Post/IPostPipeline.hpp"

namespace acid
{
	class ACID_EXPORT PipelineGaussian :
		public IPostPipeline
	{
	private:
		FilterBlur m_filterBlurVertical;
		FilterBlur m_filterBlurHorizontal;

	//	std::unique_ptr<Texture> m_halfRes;
	//	uint32_t m_lastWidth;
	//	uint32_t m_lastHeight;
	public:
		explicit PipelineGaussian(const GraphicsStage &graphicsStage, const float &scale = 2.0f, const BlurType &blurType = BLUR_TYPE_9);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
