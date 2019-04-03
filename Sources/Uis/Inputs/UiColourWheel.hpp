#pragma once

#include "Audio/Sound.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiColourWheel :
	public UiObject
{
public:
	explicit UiColourWheel(UiObject *parent, const Colour &value = Colour::White,
		const UiBound &rectangle = UiBound(Vector2f(0.0f, 0.0f), UiReference::Centre, UiAspect::Position | UiAspect::Size));

	void UpdateObject() override;

	const Colour &GetValue() const { return m_value; }

	void SetValue(const Colour &value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(Colour)> &OnValue() { return m_onValue; }

private:
	Gui m_background;
	Sound m_soundClick;

	Colour m_value;

	Delegate<void(Colour)> m_onValue;
};
}
