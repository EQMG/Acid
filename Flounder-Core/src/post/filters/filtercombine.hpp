#pragma once

#include "../../devices/Display.hpp"
#include "../../maths/vector4.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
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

		void setSlideSpaceValue(const vector4 &slideSpaceValue) const { m_slideSpaceValue->set(slideSpaceValue); }

		void setSlideSpace(const float &x, const float &y, const float &z, const float &w) const { m_slideSpaceValue->set(x, y, z, w); }
	};
}
