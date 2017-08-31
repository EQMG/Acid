#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
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
