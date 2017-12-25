#pragma once

#include "../IPostFilter.hpp"
#include "../../Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	class FilterPixel :
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
		FilterPixel(const int &subpass);

		~FilterPixel();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;

		float GetPixelSize() const { return m_pixelSize; }

		void SetPixelSize(const float &pixelSize) { m_pixelSize = pixelSize; }
	};
}
