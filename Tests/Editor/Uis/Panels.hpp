#pragma once

#include "Hierarchy.hpp"
#include "Inspector.hpp"

using namespace acid;

namespace test
{
	class Panels :
		public UiObject
	{
	public:
		explicit Panels(UiObject *parent);

		void UpdateObject() override;
	private:
		std::unique_ptr<Hierarchy> m_hierarchy;
		std::unique_ptr<Inspector> m_inspector;
	};
}
