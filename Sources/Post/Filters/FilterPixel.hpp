#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterPixel :
		public IPostFilter
	{
	private:
		PushHandler m_pushScene;

		float m_pixelSize;
	public:
		explicit FilterPixel(const GraphicsStage &graphicsStage, const float &pixelSize = 2.0f);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetPixelSize() const { return m_pixelSize; }

		void SetPixelSize(const float &pixelSize) { m_pixelSize = pixelSize; }
	};
}
