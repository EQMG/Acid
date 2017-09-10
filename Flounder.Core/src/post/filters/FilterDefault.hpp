#pragma once

#include "../../devices/Display.hpp"
#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterDefault :
		public IPostFilter
	{
	public:
		FilterDefault();

		~FilterDefault();

		void StoreValues() override;
	};
}
