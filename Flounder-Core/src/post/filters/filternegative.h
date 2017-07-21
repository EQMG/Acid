#pragma once

#include "../ipostfilter.h"

namespace flounder
{
	class filternegative :
		public ipostfilter
	{
	public:
		filternegative();

		~filternegative();

		void storeValues() override;
	};
}
