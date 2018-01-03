#pragma once

#include <Uis/UiObject.hpp>

using namespace Flounder;

namespace Demo
{
	class UiInventory :
		public UiObject
	{
	private:
	public:
		UiInventory(UiObject *parent);

		~UiInventory();

		virtual void UpdateObject() override;
	};
}
