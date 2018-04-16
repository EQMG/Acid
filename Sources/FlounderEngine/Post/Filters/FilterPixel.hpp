#pragma once

#include "Post/IPostFilter.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	class F_EXPORT FilterPixel :
		public IPostFilter
	{
	private:
		struct UboScene
		{
			float pixelSize;
		};

		UniformBuffer *m_uniformScene;

		float m_pixelSize;
	public:
		FilterPixel(const GraphicsStage &graphicsStage);

		~FilterPixel();

		void Render(const VkCommandBuffer &commandBuffer) override;

		float GetPixelSize() const { return m_pixelSize; }

		void SetPixelSize(const float &pixelSize) { m_pixelSize = pixelSize; }
	};
}
