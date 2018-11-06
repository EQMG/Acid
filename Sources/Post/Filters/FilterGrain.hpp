#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterGrain :
		public IPostFilter
	{
	private:
		PushHandler m_pushScene;

		float m_strength;
	public:
		explicit FilterGrain(const GraphicsStage &graphicsStage, const float &strength = 2.3f);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetStrength() const { return m_strength; }

		void SetStrength(const float &strength) { m_strength = strength; }
	};
}
