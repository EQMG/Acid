#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterDarken :
		public IPostFilter
	{
	private:
		PushHandler m_pushScene;

		float m_factor;
	public:
		explicit FilterDarken(const GraphicsStage &graphicsStage, const float &factor = 0.5f);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetFactor() const { return m_factor; }

		void SetFactor(const float &factor) { m_factor = factor; }
	};
}
