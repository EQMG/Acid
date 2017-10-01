#pragma once

#include "../../maths/Matrix4.hpp"
#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterMotion :
		public IPostFilter
	{
	private:
		Matrix4 *m_lastViewMatrix;
	public:
		FilterMotion();

		~FilterMotion();

		void StoreValues() override;
	};
}
