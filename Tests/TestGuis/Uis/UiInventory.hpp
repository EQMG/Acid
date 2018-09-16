#pragma once

#include <Uis/UiObject.hpp>

using namespace acid;

namespace test
{
	class UiInventory :
		public UiObject
	{
	public:
		UiInventory(UiObject *parent);

		virtual void UpdateObject() override;
	};
}
