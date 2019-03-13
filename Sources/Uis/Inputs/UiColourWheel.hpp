#pragma once

#include "Audio/Sound.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiColourWheel : public UiObject
{
  public:
	UiColourWheel(UiObject* parent, const Colour& colour = Colour::Red, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	void UpdateObject() override;

	const Colour& GetColour() const
	{
		return m_colour;
	}

	void SetColour(const Colour& colour);

	Delegate<void(UiColourWheel*, Colour)>& GetOnPressed()
	{
		return m_onPressed;
	}

  private:
	Gui m_background;
	Sound m_soundClick;

	Colour m_colour;
	bool m_mouseOver;

	Delegate<void(UiColourWheel*, Colour)> m_onPressed;
};
}
