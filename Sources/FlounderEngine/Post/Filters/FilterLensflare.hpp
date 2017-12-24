#pragma once

#include "../../Maths/Vector3.hpp"
#include "../../Renderer/Buffers/UniformBuffer.hpp"
#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterLensflare :
		public IPostFilter
	{
	private:
		struct UboScene
		{
			Vector3 sunPosition;
			float worldHeight;
			Vector2 displaySize;
		};

		UniformBuffer *m_uniformScene;

		Vector3 *m_sunPosition;
		float m_sunHeight;
	public:
		FilterLensflare(const int &subpass);

		~FilterLensflare();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;

		void SetSunPosition(const Vector3 &sunPosition) const;

		Vector3 *GetSunPosition() const { return m_sunPosition; }

		float GetSunHeight() const { return m_sunHeight; }

		void SetSunHeight(const float &sunHeight) { m_sunHeight = sunHeight; }
	};
}
