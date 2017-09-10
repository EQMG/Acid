#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterBloom2 :
		public IPostFilter
	{
	public:
		FilterBloom2();

		~FilterBloom2();

		void StoreValues() override;
	};
}
