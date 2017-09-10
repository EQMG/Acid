#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
{
	class filtertone :
		public IPostFilter
	{
	public:
		filtertone();

		~filtertone();

		void StoreValues() override;
	};
}
