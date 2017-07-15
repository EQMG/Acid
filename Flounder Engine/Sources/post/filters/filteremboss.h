#pragma once

#include "../ipostfilter.h"

namespace flounder {
	class filteremboss :
		public ipostfilter
	{
	public:
		filteremboss();

		~filteremboss();

		void storeValues() override;
	};
}
