#pragma once

#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid {
enum class ScrollBar {
	None = 0,
	Vertical = 1,
	Horizontal = 2,
	Both = Vertical | Horizontal
};

ENABLE_BITMASK_OPERATORS(ScrollBar);

class ACID_EXPORT UiScrollBar : public UiObject {
public:
	UiScrollBar(UiObject *parent, ScrollBar type, const UiTransform &transform = {Vector2ui(Size)});

	void UpdateObject() override;

	float GetProgress();
	void SetSize(const Vector2f &size);

	static const uint32_t Size;

private:
	float ScrollByDelta(float delta) const;

	float ScrollByPosition(float position) const;

	Gui m_background;
	Gui m_scroll;
	uint32_t m_index;
	bool m_updating = false;
	bool m_mouseOver = false;
};
}
