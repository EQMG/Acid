#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterTiltshift :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		float m_blurAmount;
		float m_centre;
		float m_stepSize;
		float m_steps;
	public:
		FilterTiltshift(const GraphicsStage &graphicsStage);

		~FilterTiltshift();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetBlurAmount() const { return m_blurAmount; }

		void SetBlurAmount(const float &blurAmount) { m_blurAmount = blurAmount; }

		float GetCentre() const { return m_centre; }

		void SetCentre(const float &centre) { m_centre = centre; }

		float GetStepSize() const { return m_stepSize; }

		void SetStepSize(const float &stepSize) { m_stepSize = stepSize; }

		float GetSteps() const { return m_steps; }

		void SetSteps(const float &steps) { m_steps = steps; }
	};
}
