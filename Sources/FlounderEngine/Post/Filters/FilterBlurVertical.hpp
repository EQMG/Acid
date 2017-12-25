#pragma once

#include <Renderer/Buffers/UniformBuffer.hpp>
#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterBlurVertical :
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
		FilterBlurVertical(const int &subpass, const float &sizeScalar);

		FilterBlurVertical(const int &subpass, const int &width, const int &height);

		~FilterBlurVertical();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;

		float GetBlurAmount() const { return m_blurAmount; }

		void SetBlurAmount(const float &blurAmount) { m_blurAmount = blurAmount; }
	};
}
