#pragma once

#include "../../devices/Display.hpp"
#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterBlurVertical :
		public IPostFilter
	{
	private:
		int m_heightValue;
		float m_scaleValue;
		bool m_fitToDisplay;
		float m_sizeScalar;
	public:
		FilterBlurVertical(const float &sizeScalar);

		FilterBlurVertical(const int &width, const int &height);

		~FilterBlurVertical();

		void StoreValues() override;

		float GetScaleValue() const { return m_scaleValue; }

		void SetScaleValue(const float &scaleValue) { m_scaleValue = scaleValue; }
	};
}
