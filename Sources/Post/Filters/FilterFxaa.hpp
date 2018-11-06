#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterFxaa :
		public IPostFilter
	{
	private:
		PushHandler m_pushScene;

		float m_spanMax;
	public:
		explicit FilterFxaa(const GraphicsStage &graphicsStage, const float &spanMax = 8.0f);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetSpanMax() const { return m_spanMax; }

		void SetSpanMax(const float &spanMax) { m_spanMax = spanMax; }
	};
}
