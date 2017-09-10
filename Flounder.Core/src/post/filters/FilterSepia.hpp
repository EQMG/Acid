#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterSepia :
		public IPostFilter
	{
	public:
		FilterSepia();

		~FilterSepia();

		void StoreValues() override;
	};
}
