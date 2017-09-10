#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
{
	class filteremboss :
		public IPostFilter
	{
	public:
		filteremboss();

		~filteremboss();

		void StoreValues() override;
	};
}
