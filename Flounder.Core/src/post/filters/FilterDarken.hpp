#pragma once

#include "../../devices/Display.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterdarken :
		public IPostFilter
	{
	private:
		float m_factorValue;
	public:
		filterdarken(const float &factorValue);

		filterdarken();

		~filterdarken();

		void StoreValues() override;

		void setFactorValue(const float &factorValue) { m_factorValue = factorValue; }
	};
}
