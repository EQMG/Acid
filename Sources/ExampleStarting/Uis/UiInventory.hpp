#pragma once

#include <Uis/UiObject.hpp>

using namespace Flounder;

namespace Demo
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
