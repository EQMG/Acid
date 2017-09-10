#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterbloom2 :
		public IPostFilter
	{
	public:
		filterbloom2();

		~filterbloom2();

		void StoreValues() override;
	};
}
