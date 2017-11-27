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
		FilterDarken(const int &subpass);

		~FilterDarken();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;

		float GetFactorValue() const { return m_factorValue; }

		void SetFactorValue(const float &factorValue) { m_factorValue = factorValue; }
	};
}
