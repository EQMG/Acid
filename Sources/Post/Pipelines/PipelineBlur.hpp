#pragma once

#include "Post/Filters/FilterBlur.hpp"
#include "Post/IPostPipeline.hpp"

namespace acid
{
	class ACID_EXPORT PipelineBlur :
		public IPostPipeline
	{
	private:
		FilterBlur m_filterBlurVertical;
		FilterBlur m_filterBlurHorizontal;

		float m_scale;
		float m_blur;

		std::unique_ptr<Texture> m_output;
		uint32_t m_lastWidth;
		uint32_t m_lastHeight;
	public:
		explicit PipelineBlur(const GraphicsStage &graphicsStage, const float &scale = 0.5f, const float &blur = 2.0f, const BlurType &blurType = BLUR_TYPE_9);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetScale() const { return m_scale; }

		void SetScale(const float &scale) { m_scale = scale; }

		float GetBlur() const { return m_blur; }

		void SetBlur(const float &blur) { m_blur = blur; }

		Texture *GetOutput() const { return m_output.get(); }
	};
}
