#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterWobble :
		public PostFilter
	{
	private:
		PushHandler m_pushScene;

		float m_wobbleSpeed;
		float m_wobbleAmount;
	public:
		explicit FilterWobble(const GraphicsStage &graphicsStage, const float &wobbleSpeed = 2.0f);

		void Render(const CommandBuffer &commandBuffer, const Camera &camera) override;

		float GetWobbleSpeed() const { return m_wobbleSpeed; }

		void SetWobbleSpeed(const float &wobbleSpeed) { m_wobbleSpeed = wobbleSpeed; }
	};
}
