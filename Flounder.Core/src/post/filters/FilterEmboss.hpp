#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterEmboss :
		public IPostFilter
	{
	public:
		FilterEmboss();

		~FilterEmboss();

		void StoreValues() override;
	};
}
