#pragma once

#include <Flounder.hpp>

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
