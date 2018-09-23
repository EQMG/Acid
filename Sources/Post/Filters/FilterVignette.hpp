#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterVignette :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		float m_innerRadius;
		float m_outerRadius;
		float m_opacity;
	public:
		explicit FilterVignette(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetInnerRadius() const { return m_innerRadius; }

		void SetInnerRadius(const float &innerRadius) { m_innerRadius = innerRadius; }

		float GetOuterRadius() const { return m_outerRadius; }

		void SetOuterRadius(const float &outerRadius) { m_outerRadius = outerRadius; }

		float GetOpacity() const { return m_opacity; }

		void SetOpacity(const float &opacity) { m_opacity = opacity; }
	};
}
