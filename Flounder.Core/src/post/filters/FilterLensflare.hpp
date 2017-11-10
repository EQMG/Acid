#pragma once

#include "../../maths/Vector3.hpp"
#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterLensflare :
		public IPostFilter
	{
	private:
		Vector3 *m_sunPosition;
		float m_sunHeight;
	public:
		FilterLensflare();

		~FilterLensflare();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;

		void SetSunPosition(const Vector3 &sunPosition) const;

		Vector3 *GetSunPosition() const { return m_sunPosition; }

		float GetSunHeight() const { return m_sunHeight; }

		void SetSunHeight(const float &sunHeight) { m_sunHeight = sunHeight; }
	};
}
