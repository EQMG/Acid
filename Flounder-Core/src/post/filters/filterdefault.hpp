#pragma once

#include "../../devices/display.hpp"
#include "../ipostfilter.hpp"

namespace flounder
{
	class filterdefault :
		public ipostfilter
	{
	public:
		filterdefault();

		~filterdefault();

		void storeValues() override;
	};
}
