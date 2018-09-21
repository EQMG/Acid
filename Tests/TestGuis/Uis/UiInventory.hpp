#pragma once

#include <Uis/UiObject.hpp>

using namespace acid;

namespace test
{
	class UiInventory :
		public UiObject
	{
	public:
		explicit UiInventory(UiObject *parent);

		virtual void UpdateObject() override;
	};
}
