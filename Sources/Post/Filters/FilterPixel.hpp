#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterPixel :
		public PostFilter
	{
	private:
		PushHandler m_pushScene;

		float m_pixelSize;
	public:
		explicit FilterPixel(const GraphicsStage &graphicsStage, const float &pixelSize = 2.0f);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera) override;

		float GetPixelSize() const { return m_pixelSize; }

		void SetPixelSize(const float &pixelSize) { m_pixelSize = pixelSize; }
	};
}
