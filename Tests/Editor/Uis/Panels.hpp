#pragma once

#include "Hierarchy.hpp"
#include "Inspector.hpp"

using namespace acid;

namespace test {
class Panels : public UiObject {
public:
	Panels();

	void UpdateObject() override;

private:
	Hierarchy m_hierarchy;
	Inspector m_inspector;
};
}
