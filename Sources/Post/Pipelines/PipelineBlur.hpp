#pragma once

#include "Post/Filters/FilterBlur.hpp"
#include "Post/PostPipeline.hpp"

namespace acid
{
	class ACID_EXPORT PipelineBlur :
		public PostPipeline
	{
	private:
		FilterBlur m_filterBlurVertical;
		FilterBlur m_filterBlurHorizontal;

		bool m_toScreen;
		float m_inputScale;
		float m_outputScale;
		float m_blur;

		std::unique_ptr<Texture> m_output;
		uint32_t m_lastWidth;
		uint32_t m_lastHeight;
	public:
		explicit PipelineBlur(const GraphicsStage &graphicsStage, const float &blur = 2.0f, const BlurType &blurType = BLUR_TYPE_9,
			const bool &toScreen = false, const float &inputScale = 0.5f, const float &outputScale = 1.0f);

		void Render(const CommandBuffer &commandBuffer, const Camera &camera) override;

		const float &GetInputScale() const { return m_inputScale; }

		void SetInputScale(const float &inputScale) { m_inputScale = inputScale; }

		const float &GetOutputScale() const { return m_outputScale; }

		void SetOutputScale(const float &outputScale) { m_outputScale = outputScale; }

		const float &GetBlur() const { return m_blur; }

		void SetBlur(const float &blur) { m_blur = blur; }

		const Texture *GetOutput() const { return m_output.get(); }
	};
}
