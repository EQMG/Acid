#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class FL_EXPORT FilterBlurHorizontal :
		public IPostFilter
	{
	private:
		UniformHandler *m_uniformScene;

		float m_scale;
		float m_width;
	public:
		FilterBlurHorizontal(const GraphicsStage &graphicsStage, const float &scale = 2.0f);

		~FilterBlurHorizontal();

		void Render(const CommandBuffer &commandBuffer) override;
	};
}
