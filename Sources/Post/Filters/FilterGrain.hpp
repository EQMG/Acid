#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterGrain :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		float m_strength;
	public:
		FilterGrain(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetStrength() const { return m_strength; }

		void SetStrength(const float &strength) { m_strength = strength; }
	};
}
