#pragma once

#include "Maths/Vector3.hpp"
#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterLensflare :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		Vector3 m_sunPosition;
		float m_sunHeight;
	public:
		explicit FilterLensflare(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		Vector3 GetSunPosition() const { return m_sunPosition; }

		void SetSunPosition(const Vector3 &sunPosition);

		float GetSunHeight() const { return m_sunHeight; }

		void SetSunHeight(const float &sunHeight) { m_sunHeight = sunHeight; }
	};
}
