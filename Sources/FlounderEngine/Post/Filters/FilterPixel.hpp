#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class F_EXPORT FilterPixel :
		public IPostFilter
	{
	private:
		UniformHandler *m_uniformScene;

		float m_pixelSize;
	public:
		FilterPixel(const GraphicsStage &graphicsStage);

		~FilterPixel();

		void Render(const VkCommandBuffer &commandBuffer) override;

		float GetPixelSize() const { return m_pixelSize; }

		void SetPixelSize(const float &pixelSize) { m_pixelSize = pixelSize; }
	};
}
