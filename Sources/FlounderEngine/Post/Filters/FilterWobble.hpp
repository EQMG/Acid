#pragma once

#include "../IPostFilter.hpp"
#include "../../Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	class F_EXPORT FilterWobble :
		public IPostFilter
	{
	private:
		struct UboScene
		{
			float moveIt;
		};

		UniformBuffer *m_uniformScene;

		float m_wobbleSpeed;
		float m_wobbleAmount;
	public:
		FilterWobble(const GraphicsStage &graphicsStage);

		~FilterWobble();

		void Render(const VkCommandBuffer &commandBuffer) override;

		float GetWobbleSpeed() const { return m_wobbleSpeed; }

		void SetWobbleSpeed(const float &wobbleSpeed) { m_wobbleSpeed = wobbleSpeed; }
	};
}
