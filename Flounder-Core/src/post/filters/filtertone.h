#pragma once

#include "../ipostfilter.h"

namespace flounder
{
	class filtertone :
		public ipostfilter
	{
	public:
		filtertone();

		~filtertone();

		void storeValues() override;
	};
}
