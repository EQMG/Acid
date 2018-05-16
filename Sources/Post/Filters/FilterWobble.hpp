#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class FL_EXPORT FilterWobble :
		public IPostFilter
	{
	private:
		UniformHandler *m_uniformScene;

		float m_wobbleSpeed;
		float m_wobbleAmount;
	public:
		FilterWobble(const GraphicsStage &graphicsStage);

		~FilterWobble();

		void Render(const CommandBuffer &commandBuffer) override;

		float GetWobbleSpeed() const { return m_wobbleSpeed; }

		void SetWobbleSpeed(const float &wobbleSpeed) { m_wobbleSpeed = wobbleSpeed; }
	};
}
