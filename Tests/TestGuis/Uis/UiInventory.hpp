#pragma once

#include <Uis/UiObject.hpp>

using namespace fl;

namespace test
{
	class UiInventory :
		public UiObject
	{
	public:
		UiInventory(UiObject *parent);

		~UiInventory();

		virtual void UpdateObject() override;
	};
}
