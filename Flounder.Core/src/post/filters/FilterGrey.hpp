#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
{
	class filtergrey :
		public IPostFilter
	{
	public:
		filtergrey();

		~filtergrey();

		void StoreValues() override;
	};
}
