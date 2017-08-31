#pragma once

#include "../../devices/display.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
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
