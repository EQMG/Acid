#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
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
