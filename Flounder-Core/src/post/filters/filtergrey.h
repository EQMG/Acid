#pragma once

#include "../ipostfilter.h"

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
