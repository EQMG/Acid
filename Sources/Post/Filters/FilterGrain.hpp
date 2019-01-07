#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterGrain :
		public PostFilter
	{
	private:
		PushHandler m_pushScene;

		float m_strength;
	public:
		explicit FilterGrain(const GraphicsStage &graphicsStage, const float &strength = 2.3f);

		void Render(const CommandBuffer &commandBuffer, const Camera &camera) override;

		const float &GetStrength() const { return m_strength; }

		void SetStrength(const float &strength) { m_strength = strength; }
	};
}
