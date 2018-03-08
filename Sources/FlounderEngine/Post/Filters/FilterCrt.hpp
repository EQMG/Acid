#pragma once

#include "../../Maths/Colour.hpp"
#include "../../Renderer/Buffers/UniformBuffer.hpp"
#include "../IPostFilter.hpp"

namespace Flounder
{
	class F_EXPORT FilterCrt :
		public IPostFilter
	{
	private:
		struct UboScene
		{
			Colour screenColour;
			float curveAmountX;
			float curveAmountY;
			float scanLineSize;
			float scanIntensity;
			float moveTime;
		};

		UniformBuffer *m_uniformScene;

		Colour *m_screenColour;
		float m_curveAmountX;
		float m_curveAmountY;
		float m_scanLineSize;
		float m_scanIntensity;
	public:
		FilterCrt(const GraphicsStage &graphicsStage);

		~FilterCrt();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;

		Colour *GetScreenColour() const { return m_screenColour; }

		void SetScreenColour(const Colour &screenColour) const { m_screenColour->Set(screenColour); }

		float GetCurveAmountX() const { return m_curveAmountX; }

		void SetCurveAmountX(const float &curveAmountX) { m_curveAmountX = curveAmountX; }

		float GetCurveAmountY() const { return m_curveAmountY; }

		void SetCurveAmountY(const float &curveAmountY) { m_curveAmountY = curveAmountY; }

		float GetScanLineSize() const { return m_scanLineSize; }

		void SetScanLineSize(const float &scanLineSize) { m_scanLineSize = scanLineSize; }

		float GetScanIntensity() const { return m_scanIntensity; }

		void SetScanIntensity(const float &scanIntensity) { m_scanIntensity = scanIntensity; }
	};
}
