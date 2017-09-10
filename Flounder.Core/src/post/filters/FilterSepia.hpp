#pragma once

#include "../ipostfilter.hpp"

namespace Flounder
{
	class filtersepia :
		public IPostFilter
	{
	public:
		filtersepia();

		~filtersepia();

		void StoreValues() override;
	};
}
