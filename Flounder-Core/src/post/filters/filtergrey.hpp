#pragma once

#include "../ipostfilter.hpp"

namespace flounder
{
	class filtergrey :
		public ipostfilter
	{
	public:
		filtergrey();

		~filtergrey();

		void storeValues() override;
	};
}
