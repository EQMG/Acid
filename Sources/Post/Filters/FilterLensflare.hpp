#pragma once

#include "Maths/Vector3.hpp"
#include "Post/IPostFilter.hpp"

namespace fl
{
	class FL_EXPORT FilterLensflare :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		Vector3 m_sunPosition;
		float m_sunHeight;
	public:
		FilterLensflare(const GraphicsStage &graphicsStage);

		~FilterLensflare();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		Vector3 GetSunPosition() const { return m_sunPosition; }

		void SetSunPosition(const Vector3 &sunPosition);

		float GetSunHeight() const { return m_sunHeight; }

		void SetSunHeight(const float &sunHeight) { m_sunHeight = sunHeight; }
	};
}
