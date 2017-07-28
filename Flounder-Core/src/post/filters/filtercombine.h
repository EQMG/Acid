#pragma once

#include "../../devices/display.h"
#include "../../maths/vector4.h"
#include "../ipostfilter.h"

namespace flounder
{
	class filtercombine :
		public ipostfilter
	{
	private:
		vector4 *m_slideSpaceValue;
	public:
		filtercombine();

		~filtercombine();

		void storeValues() override;

		inline void setSlideSpaceValue(const vector4 &slideSpaceValue) const { m_slideSpaceValue->set(slideSpaceValue); }

		void setSlideSpace(const float &x, const float &y, const float &z, const float &w) const { m_slideSpaceValue->set(x, y, z, w); }
	};
}
