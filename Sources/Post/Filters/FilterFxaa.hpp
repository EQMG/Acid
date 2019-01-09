#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterFxaa :
		public PostFilter
	{
	private:
		PushHandler m_pushScene;

		float m_spanMax;
	public:
		explicit FilterFxaa(const GraphicsStage &graphicsStage, const float &spanMax = 8.0f);

		void Render(const CommandBuffer &commandBuffer) override;

		const float &GetSpanMax() const { return m_spanMax; }

		void SetSpanMax(const float &spanMax) { m_spanMax = spanMax; }
	};
}
