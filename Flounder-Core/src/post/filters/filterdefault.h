#pragma once

#include "../../devices/display.h"
#include "../ipostfilter.h"

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
