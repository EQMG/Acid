#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
{
	class filternegative :
		public IPostFilter
	{
	public:
		filternegative();

		~filternegative();

		void StoreValues() override;
	};
}
