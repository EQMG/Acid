#pragma once

#include "../ipostfilter.h"

namespace flounder
{
	class filterbloom1 :
		public ipostfilter
	{
	public:
		filterbloom1();

		~filterbloom1();

		void storeValues() override;
	};
}
