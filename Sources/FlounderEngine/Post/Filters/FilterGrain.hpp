#pragma once

#include "Post/IPostFilter.hpp"

namespace Flounder
{
	class F_EXPORT FilterGrain :
		public IPostFilter
	{
	private:
		UniformHandler *m_uniformScene;

		float m_strength;
	public:
		FilterGrain(const GraphicsStage &graphicsStage);

		~FilterGrain();

		void Render(const VkCommandBuffer &commandBuffer) override;

		float GetStrength() const { return m_strength; }

		void SetStrength(const float &strength) { m_strength = strength; }
	};
}
