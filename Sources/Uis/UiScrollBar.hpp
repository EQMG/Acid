#pragma once

#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
enum class ScrollBar
{
	None = 0,
	Vertical = 1,
	Horizontal = 2
};

ENABLE_BITMASK_OPERATORS(ScrollBar)

class ACID_EXPORT UiScrollBar : public UiObject
{
  public:
	UiScrollBar(UiObject* parent, const ScrollBar& type, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	void UpdateObject() override;

	float GetProgress();

  private:
	float ScrollByDelta(const float& delta);

	float ScrollByPosition(const float& position);

	Gui m_scroll;
	uint32_t m_index;
	bool m_updating;
	bool m_mouseOver;
};
}
