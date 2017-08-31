#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
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
