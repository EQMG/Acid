#pragma once

#include "../../devices/Display.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterdefault :
		public IPostFilter
	{
	public:
		filterdefault();

		~filterdefault();

		void StoreValues() override;
	};
}
