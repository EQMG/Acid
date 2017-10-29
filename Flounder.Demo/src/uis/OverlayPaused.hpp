#pragma once

#include <Flounder.hpp>

using namespace Flounder;

namespace Demo
{
	class OverlayPaused :
		public UiObject
	{
	private:
	public:
		OverlayPaused(UiObject *parent);

		~OverlayPaused();

		virtual void UpdateObject() override;
	};
}
