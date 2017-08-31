#pragma once

#include "../../devices/Display.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
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
