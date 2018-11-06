#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterTiltshift :
		public IPostFilter
	{
	private:
		PushHandler m_pushScene;

		float m_blurAmount;
		float m_centre;
		float m_stepSize;
		float m_steps;
	public:
		explicit FilterTiltshift(const GraphicsStage &graphicsStage, const float &blurAmount = 1.0f, const float &centre = 1.1f, const float &stepSize = 0.004f, const float &steps = 3.0f);

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
