#pragma once

#include "../ipostfilter.hpp"

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
