#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
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
