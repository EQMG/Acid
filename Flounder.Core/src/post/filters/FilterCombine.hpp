#pragma once

#include "../../devices/Display.hpp"
#include "../../maths/Vector4.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filtercombine :
		public ipostfilter
	{
	private:
		Vector4 *m_slideSpaceValue;
	public:
		filtercombine();

		~filtercombine();

		void storeValues() override;

		void setSlideSpaceValue(const Vector4 &slideSpaceValue) const { m_slideSpaceValue->Set(slideSpaceValue); }

		void setSlideSpace(const float &x, const float &y, const float &z, const float &w) const { m_slideSpaceValue->Set(x, y, z, w); }
	};
}
