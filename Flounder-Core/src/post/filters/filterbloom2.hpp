#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
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
