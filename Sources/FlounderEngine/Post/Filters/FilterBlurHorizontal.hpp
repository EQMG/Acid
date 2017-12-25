#pragma once

#include "../IPostFilter.hpp"
#include "../../Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	class FilterBlurHorizontal :
		public IPostFilter
	{
	private:
		struct UboScene
		{
			float blurAmount;
			Vector2 size;
		};

		UniformBuffer *m_uniformScene;

		Vector2 m_size;
		float m_blurAmount;
		bool m_fitToDisplay;
		float m_sizeScalar;
	public:
		FilterBlurHorizontal(const int &subpass, const float &sizeScalar);

		FilterBlurHorizontal(const int &subpass, const int &width, const int &height);

		~FilterBlurHorizontal();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;

		float GetBlurAmount() const { return m_blurAmount; }

		void SetBlurAmount(const float &blurAmount) { m_blurAmount = blurAmount; }
	};
}
