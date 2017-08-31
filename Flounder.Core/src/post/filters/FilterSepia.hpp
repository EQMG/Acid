#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
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
