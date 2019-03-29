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
	Hierarchy m_hierarchy;
	Inspector m_inspector;
};
}
