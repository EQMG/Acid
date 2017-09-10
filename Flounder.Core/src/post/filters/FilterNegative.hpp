#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterNegative :
		public IPostFilter
	{
	public:
		FilterNegative();

		~FilterNegative();

		void StoreValues() override;
	};
}
