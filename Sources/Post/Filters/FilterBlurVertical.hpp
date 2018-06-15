#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class FL_EXPORT FilterBlurVertical :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		float m_scale;
		float m_height;
	public:
		FilterBlurVertical(const GraphicsStage &graphicsStage, const float &scale = 2.0f);

		~FilterBlurVertical();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
