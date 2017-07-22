#pragma once

#include "../ipostfilter.h"

namespace flounder
{
	class filtersepia :
		public ipostfilter
	{
	public:
		filtersepia();

		~filtersepia();

		void storeValues() override;
	};
}
