#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterDarken :
		public IPostFilter
	{
	private:
		float m_factorValue;
	public:
		FilterDarken(const float &factorValue);

		FilterDarken();

		~FilterDarken();

		void StoreValues() override;

		float GetFactorValue() const { return m_factorValue; }

		void SetFactorValue(const float &factorValue) { m_factorValue = factorValue; }
	};
}
