#pragma once

#include "../../devices/display.h"
#include "../ipostfilter.h"

namespace flounder
{
	class filterdarken :
		public ipostfilter
	{
	private:
		float m_factorValue;
	public:
		filterdarken(const float &factorValue);

		filterdarken();

		~filterdarken();

		void storeValues() override;

		inline void setFactorValue(const float &factorValue) { m_factorValue = factorValue; }
	};
}
