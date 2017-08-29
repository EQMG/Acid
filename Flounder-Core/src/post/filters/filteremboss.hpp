#pragma once

#include "../ipostfilter.hpp"

namespace flounder
{
	class filteremboss :
		public ipostfilter
	{
	public:
		filteremboss();

		~filteremboss();

		void storeValues() override;
	};
}
