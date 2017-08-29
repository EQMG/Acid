#pragma once

#include "../ipostfilter.hpp"

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
