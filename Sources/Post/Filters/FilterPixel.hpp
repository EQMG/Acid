#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterPixel :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		float m_pixelSize;
	public:
		FilterPixel(const GraphicsStage &graphicsStage);

		~FilterPixel();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetPixelSize() const { return m_pixelSize; }

		void SetPixelSize(const float &pixelSize) { m_pixelSize = pixelSize; }
	};
}
