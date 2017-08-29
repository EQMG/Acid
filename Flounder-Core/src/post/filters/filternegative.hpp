#pragma once

#include "../ipostfilter.hpp"

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
