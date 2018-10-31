#pragma once

#include "Maths/Colour.hpp"
#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterDof :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		float m_maxBlurSize;
		float m_radiusScale;
		float m_goldenAngle;
	public:
		explicit FilterDof(const GraphicsStage &graphicsStage, const float &maxBlurSize = 20.0f, const float &radiusScale = 0.5f, const float &goldenAngle = 2.39996323f);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetMaxBlurSize() const { return m_maxBlurSize; }

		void SetMaxBlurSize(const float &maxBlurSize) { m_maxBlurSize = maxBlurSize; }

		float GetRadiusScale() const { return m_radiusScale; }

		void SetRadiusScale(const float &radiusScale) { m_radiusScale = radiusScale; }

		float GetGoldenAngle() const { return m_goldenAngle; }

		void SetGoldenAngle(const float &goldenAngle) { m_goldenAngle = goldenAngle; }
	};
}
