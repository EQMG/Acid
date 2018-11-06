#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterVignette :
		public IPostFilter
	{
	private:
		PushHandler m_pushScene;

		float m_innerRadius;
		float m_outerRadius;
		float m_opacity;
	public:
		explicit FilterVignette(const GraphicsStage &graphicsStage, const float &innerRadius = 0.15f, const float &outerRadius = 1.35f, const float &opacity = 0.85f);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetInnerRadius() const { return m_innerRadius; }

		void SetInnerRadius(const float &innerRadius) { m_innerRadius = innerRadius; }

		float GetOuterRadius() const { return m_outerRadius; }

		void SetOuterRadius(const float &outerRadius) { m_outerRadius = outerRadius; }

		float GetOpacity() const { return m_opacity; }

		void SetOpacity(const float &opacity) { m_opacity = opacity; }
	};
}
