#pragma once

#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiSection : public UiObject
{
  public:
	UiSection(UiObject* parent, const std::string& string, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	void UpdateObject() override;

	UiObject& GetContent()
	{
		return m_content;
	}

	Delegate<void(UiSection*, bool)>& GetOnCollapsed()
	{
		return m_onCollapsed;
	}

  private:
	Gui m_icon;
	Text m_text;
	UiObject m_content;
	Sound m_soundClick;

	bool m_collapsed;

	Delegate<void(UiSection*, bool)> m_onCollapsed;
};
}
