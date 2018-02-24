#pragma once

#include "../IPostFilter.hpp"
#include "../../Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	class F_EXPORT FilterGrain :
		public IPostFilter
	{
	private:
		struct UboScene
		{
			float strength;
		};

		UniformBuffer *m_uniformScene;

		float m_strength;
	public:
		FilterGrain(const GraphicsStage &graphicsStage);

		~FilterGrain();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;

		float GetStrength() const { return m_strength; }

		void SetStrength(const float &strength) { m_strength = strength; }
	};
}
