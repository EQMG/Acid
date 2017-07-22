#pragma once

#include "../ipostfilter.h"

namespace flounder
{
	class filterbloom2 :
		public ipostfilter
	{
	public:
		filterbloom2();

		~filterbloom2();

		void storeValues() override;
	};
}
