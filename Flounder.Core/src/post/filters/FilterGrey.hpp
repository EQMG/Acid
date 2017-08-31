#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
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
