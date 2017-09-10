#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterbloom1 :
		public IPostFilter
	{
	public:
		filterbloom1();

		~filterbloom1();

		void StoreValues() override;
	};
}
